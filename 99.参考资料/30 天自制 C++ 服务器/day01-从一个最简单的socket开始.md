# day01-从一个最简单的 socket 开始

如果读者之前有计算机网络的基础知识那就更好了，没有也没关系，socket 编程非常容易上手。但本教程主要偏向实践，不会详细讲述计算机网络协议、网络编程原理等。想快速入门可以看以下博客，讲解比较清楚、错误较少：

- [计算机网络基础知识总结](https://www.runoob.com/w3cnote/summary-of-network.html)

要想打好基础，抄近道是不可的，有时间一定要认真学一遍谢希仁的《计算机网络》，要想精通服务器开发，这必不可少。

首先在服务器，我们需要建立一个 socket 套接字，对外提供一个网络通信接口，在 Linux 系统中这个套接字竟然仅仅是一个文件描述符，也就是一个`int`类型的值！这个对套接字的所有操作（包括创建）都是最底层的系统调用。

> 在这里读者务必先了解什么是 Linux 系统调用和文件描述符，《现代操作系统》第四版第一章有详细的讨论。如果你想抄近道看博客，C 语言中文网的这篇文章讲了一部分：[socket 是什么？套接字是什么？](http://c.biancheng.net/view/2123.html)

> Unix 哲学 KISS：keep it simple, stupid。在 Linux 系统里，一切看上去十分复杂的逻辑功能，都用简单到不可思议的方式实现，甚至有些时候看上去很愚蠢。但仔细推敲，人们将会赞叹 Linux 的精巧设计，或许这就是大智若愚。

```cpp
#include <sys/socket.h>
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

- 第一个参数：IP 地址类型，AF_INET 表示使用 IPv4，如果使用 IPv6 请使用 AF_INET6。
- 第二个参数：数据传输方式，SOCK_STREAM 表示流格式、面向连接，多用于 TCP。SOCK_DGRAM 表示数据报格式、无连接，多用于 UDP。
- 第三个参数：协议，0 表示根据前面的两个参数自动推导协议类型。设置为 IPPROTO_TCP 和 IPPTOTO_UDP，分别表示 TCP 和 UDP。

对于客户端，服务器存在的唯一标识是一个 IP 地址和端口，这时候我们需要将这个套接字绑定到一个 IP 地址和端口上。首先创建一个 sockaddr_in 结构体

```cpp
#include <arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
struct sockaddr_in serv_addr;
bzero(&serv_addr, sizeof(serv_addr));
```

然后使用`bzero`初始化这个结构体，这个函数在头文件`<string.h>`或`<cstring>`中。这里用到了两条《Effective C++》的准则：

> 条款 04: 确定对象被使用前已先被初始化。如果不清空，使用 gdb 调试器查看 addr 内的变量，会是一些随机值，未来可能会导致意想不到的问题。

> 条款 01: 视 C++为一个语言联邦。把 C 和 C++看作两种语言，写代码时需要清楚地知道自己在写 C 还是 C++。如果在写 C，请包含头文件`<string.h>`。如果在写 C++，请包含`<cstring>`。

设置地址族、IP 地址和端口：

```cpp
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
serv_addr.sin_port = htons(8888);
```

然后将 socket 地址与文件描述符绑定：

```cpp
bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
```

> 为什么定义的时候使用专用 socket 地址（sockaddr_in）而绑定的时候要转化为通用 socket 地址（sockaddr），以及转化 IP 地址和端口号为网络字节序的`inet_addr`和`htons`等函数及其必要性，在游双《Linux 高性能服务器编程》第五章第一节：socket 地址 API 中有详细讨论。

最后我们需要使用`listen`函数监听这个 socket 端口，这个函数的第二个参数是 listen 函数的最大监听队列长度，系统建议的最大值`SOMAXCONN`被定义为 128。

```cpp
listen(sockfd, SOMAXCONN);
```

要接受一个客户端连接，需要使用`accept`函数。对于每一个客户端，我们在接受连接时也需要保存客户端的 socket 地址信息，于是有以下代码：

```cpp
struct sockaddr_in clnt_addr;
socklen_t clnt_addr_len = sizeof(clnt_addr);
bzero(&clnt_addr, sizeof(clnt_addr));
int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
```

要注意和`accept`和`bind`的第三个参数有一点区别，对于`bind`只需要传入 serv_addr 的大小即可，而`accept`需要写入客户端 socket 长度，所以需要定义一个类型为`socklen_t`的变量，并传入这个变量的地址。另外，`accept`函数会阻塞当前程序，直到有一个客户端 socket 被接受后程序才会往下运行。

到现在，客户端已经可以通过 IP 地址和端口号连接到这个 socket 端口了，让我们写一个测试客户端连接试试：

```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in serv_addr;
bzero(&serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
serv_addr.sin_port = htons(8888);
connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
```

代码和服务器代码几乎一样：创建一个 socket 文件描述符，与一个 IP 地址和端口绑定，最后并不是监听这个端口，而是使用`connect`函数尝试连接这个服务器。

至此，day01 的教程已经结束了，进入`code/day01`文件夹，使用 make 命令编译，将会得到`server`和`client`。输入命令`./server`开始运行，直到`accept`函数，程序阻塞、等待客户端连接。然后在一个新终端输入命令`./client`运行客户端，可以看到服务器接收到了客户端的连接请求，并成功连接。

```
new client fd 3! IP: 127.0.0.1 Port: 53505
```

但如果我们先运行客户端、后运行服务器，在客户端一侧无任何区别，却并没有连接服务器成功，因为我们 day01 的程序没有任何的错误处理。

事实上对于如`socket`,`bind`,`listen`,`accept`,`connect`等函数，通过返回值以及`errno`可以确定程序运行的状态、是否发生错误。在 day02 的教程中，我们会进一步完善整个服务器，处理所有可能的错误，并实现一个 echo 服务器（客户端发送给服务器一个字符串，服务器收到后返回相同的内容）。

完整源代码：[https://github.com/yuesong-feng/30dayMakeCppServer/tree/main/code/day01](https://github.com/yuesong-feng/30dayMakeCppServer/tree/main/code/day01)
