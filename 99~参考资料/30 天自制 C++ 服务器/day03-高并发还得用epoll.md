# day03-高并发还得用 epoll

在上一天，我们写了一个简单的 echo 服务器，但只能同时处理一个客户端的连接。但在这个连接的生命周期中，绝大部分时间都是空闲的，活跃时间（发送数据和接收数据的时间）占比极少，这样独占一个服务器是严重的资源浪费。事实上所有的服务器都是高并发的，可以同时为成千上万个客户端提供服务，这一技术又被称为 IO 复用。

> IO 复用和多线程有相似之处，但绝不是一个概念。IO 复用是针对 IO 接口，而多线程是针对 CPU。

IO 复用的基本思想是事件驱动，服务器同时保持多个客户端 IO 连接，当这个 IO 上有可读或可写事件发生时，表示这个 IO 对应的客户端在请求服务器的某项服务，此时服务器响应该服务。在 Linux 系统中，IO 复用使用 select, poll 和 epoll 来实现。epoll 改进了前两者，更加高效、性能更好，是目前几乎所有高并发服务器的基石。请读者务必先掌握 epoll 的原理再进行编码开发。

> select, poll 与 epoll 的详细原理和区别请参考《UNIX 网络编程：卷 1》第二部分第六章，游双《Linux 高性能服务器编程》第九章

epoll 主要由三个系统调用组成：

```cpp
//int epfd = epoll_create(1024);  //参数表示监听事件的大小，如超过内核会自动调整，已经被舍弃，无实际意义，传入一个大于0的数即可
int epfd = epoll_create1(0);       //参数是一个flag，一般设为0，详细参考man epoll
```

创建一个 epoll 文件描述符并返回，失败则返回-1。

epoll 监听事件的描述符会放在一颗红黑树上，我们将要监听的 IO 口放入 epoll 红黑树中，就可以监听该 IO 上的事件。

```cpp
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);    //添加事件到epoll
epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);    //修改epoll红黑树上的事件
epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);   //删除事件
```

其中 sockfd 表示我们要添加的 IO 文件描述符，ev 是一个 epoll_event 结构体，其中的 events 表示事件，如 EPOLLIN 等，data 是一个用户数据 union:

```cpp
typedef union epoll_data {
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;
struct epoll_event {
  uint32_t events;	/* Epoll events */
  epoll_data_t data;	/* User data variable */
} __EPOLL_PACKED;
```

epoll 默认采用 LT 触发模式，即水平触发，只要 fd 上有事件，就会一直通知内核。这样可以保证所有事件都得到处理、不容易丢失，但可能发生的大量重复通知也会影响 epoll 的性能。如使用 ET 模式，即边缘触法，fd 从无事件到有事件的变化会通知内核一次，之后就不会再次通知内核。这种方式十分高效，可以大大提高支持的并发度，但程序逻辑必须一次性很好地处理该 fd 上的事件，编程比 LT 更繁琐。注意 ET 模式必须搭配非阻塞式 socket 使用。

> 非阻塞式 socket 和阻塞式有很大的不同，请参考《UNIX 网络编程：卷 1》第三部分第 16 章。

我们可以随时使用`epoll_wait`获取有事件发生的 fd：

```cpp
int nfds = epoll_wait(epfd, events, maxevents, timeout);
```

其中 events 是一个 epoll_event 结构体数组，maxevents 是可供返回的最大事件大小，一般是 events 的大小，timeout 表示最大等待时间，设置为-1 表示一直等待。

接下来将 day02 的服务器改写成 epoll 版本，基本思想为：在创建了服务器 socket fd 后，将这个 fd 添加到 epoll，只要这个 fd 上发生可读事件，表示有一个新的客户端连接。然后 accept 这个客户端并将客户端的 socket fd 添加到 epoll，epoll 会监听客户端 socket fd 是否有事件发生，如果发生则处理事件。

接下来的教程在伪代码中：

```cpp
int sockfd = socket(...);   //创建服务器socket fd
bind(sockfd...);
listen(sockfd...);
int epfd = epoll_create1(0);
struct epoll_event events[MAX_EVENTS], ev;
ev.events = EPOLLIN;    //在代码中使用了ET模式，且未处理错误，在day12进行了修复，实际上接受连接最好不要用ET模式
ev.data.fd = sockfd;    //该IO口为服务器socket fd
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);    //将服务器socket fd添加到epoll
while(true){    // 不断监听epoll上的事件并处理
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);   //有nfds个fd发生事件
    for(int i = 0; i < nfds; ++i){  //处理这nfds个事件
        if(events[i].data.fd == sockfd){    //发生事件的fd是服务器socket fd，表示有新客户端连接
            int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
            ev.data.fd = clnt_sockfd;
            ev.events = EPOLLIN | EPOLLET;  //对于客户端连接，使用ET模式，可以让epoll更加高效，支持更多并发
            setnonblocking(clnt_sockfd);    //ET需要搭配非阻塞式socket使用
            epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);   //将该客户端的socket fd添加到epoll
        } else if(events[i].events & EPOLLIN){      //发生事件的是客户端，并且是可读事件（EPOLLIN）
            handleEvent(events[i].data.fd);         //处理该fd上发生的事件
        }
    }
}
```

从一个非阻塞式 socket fd 上读取数据时：

```cpp
while(true){    //由于使用非阻塞IO，需要不断读取，直到全部读取完毕
    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
    if(bytes_read > 0){
      //保存读取到的bytes_read大小的数据
    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
        continue;
    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
        //该fd上数据读取完毕
        break;
    } else if(bytes_read == 0){  //EOF事件，一般表示客户端断开连接
        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
        break;
    } //剩下的bytes_read == -1的情况表示其他错误，这里没有处理
}
```

至此，day03 的主要教程已经结束了，完整源代码请在`code/day03`文件夹，接下来看看今天的学习成果以及测试我们的服务器！

进入`code/day03`文件夹，使用 make 命令编译，将会得到`server`和`client`，输入命令`./server`开始运行服务器。然后在一个新终端输入命令`./client`运行客户端，可以看到服务器接收到了客户端的连接请求，并成功连接。再新开一个或多个终端，运行 client，可以看到这些客户端也同时连接到了服务器。此时我们在任意一个 client 输入一条信息，服务器都显示并发送到该客户端。如使用`control+c`终止掉某个 client，服务器回显示这个 client 已经断开连接，但其他 client 并不受影响。

至此，我们已经完整地开发了一个 echo 服务器，并且支持多个客户端同时连接，为他们提供服务！

完整源代码：[https://github.com/yuesong-feng/30dayMakeCppServer/tree/main/code/day03](https://github.com/yuesong-feng/30dayMakeCppServer/tree/main/code/day03)
