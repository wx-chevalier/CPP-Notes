// https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
// http://www.kegel.com/poller/

/** Todo
 * - [ ] Split i/o and logic
 * - [ ] Unit test logic
 * - [ ] Logging
 * - [ ] Continuous integration
 *   - [ ] Linux
 *   - [ ] Windows
 *   - [ ] Mac
 * - [ ] Library-fy
 * - [ ] Add support for kqueue
 * - [ ] Add support for windows equivalent
 */

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <array>

namespace
{
    constexpr int max_events = 32;

    auto create_and_bind(std::string const& port)
    {
        struct addrinfo hints;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
        hints.ai_socktype = SOCK_STREAM; /* TCP */
        hints.ai_flags = AI_PASSIVE; /* All interfaces */

        struct addrinfo* result;
        int sockt = getaddrinfo(nullptr, port.c_str(), &hints, &result);
        if (sockt != 0)
        {
            std::cerr << "[E] getaddrinfo failed\n";
            return -1;
        }

        struct addrinfo* rp;
        int socketfd;
        for (rp = result; rp != nullptr; rp = rp->ai_next)
        {
            socketfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (socketfd == -1)
            {
                continue;
            }

            sockt = bind(socketfd, rp->ai_addr, rp->ai_addrlen);
            if (sockt == 0)
            {
                break;
            }

            close(socketfd);
        }

        if (rp == nullptr)
        {
            std::cerr << "[E] bind failed\n";
            return -1;
        }

        freeaddrinfo(result);

        return socketfd;
    }

    auto make_socket_nonblocking(int socketfd)
    {
        int flags = fcntl(socketfd, F_GETFL, 0);
        if (flags == -1)
        {
            std::cerr << "[E] fcntl failed (F_GETFL)\n";
            return false;
        }

        flags |= O_NONBLOCK;
        int s = fcntl(socketfd, F_SETFL, flags);
        if (s == -1)
        {
            std::cerr << "[E] fcntl failed (F_SETFL)\n";
            return false;
        }

        return true;
    }

    auto accept_connection(int socketfd, struct epoll_event& event, int epollfd)
    {
        struct sockaddr in_addr;
        socklen_t in_len = sizeof(in_addr);
        int infd = accept(socketfd, &in_addr, &in_len);
        if (infd == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) // Done processing incoming connections
            {
                return false;
            }
            else
            {
                std::cerr << "[E] accept failed\n";
                return false;
            }
        }

        std::string hbuf(NI_MAXHOST, '\0');
        std::string sbuf(NI_MAXSERV, '\0');
        if (getnameinfo(&in_addr, in_len,
                        const_cast<char*>(hbuf.data()), hbuf.size(),
                        const_cast<char*>(sbuf.data()), sbuf.size(),
                        NI_NUMERICHOST | NI_NUMERICSERV) == 0)
        {
            std::cout << "[I] Accepted connection on descriptor " << infd << "(host=" << hbuf << ", port=" << sbuf << ")" << "\n";
        }

        if (!make_socket_nonblocking(infd))
        {
            std::cerr << "[E] make_socket_nonblocking failed\n";
            return false;
        }

        event.data.fd = infd;
        event.events = EPOLLIN | EPOLLET;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, infd, &event) == -1)
        {
            std::cerr << "[E] epoll_ctl failed\n";
            return false;
        }

        return true;
    }

    auto read_data(int fd)
    {
        char buf[512];
        auto count = read(fd, buf, 512);
        if (count == -1)
        {
            if (errno == EAGAIN) // read all data
            {
                return false;
            }
        }
        else if (count == 0) // EOF - remote closed connection
        {
            std::cout << "[I] Close " << fd << "\n";
            close(fd);
            return false;
        }

        std::cout << fd << " says: " <<  buf;

        return true;
    }
}

int main()
{
    auto socketfd = ::create_and_bind("4242");
    if (socketfd == -1)
    {
        return 1;
    }

    if (!::make_socket_nonblocking(socketfd))
    {
        return 1;
    }

    if (listen(socketfd, SOMAXCONN) == -1)
    {
        std::cerr << "[E] listen failed\n";
        return 1;
    }

    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        std::cerr << "[E] epoll_create1 failed\n";
        return 1;
    }

    struct epoll_event event;
    event.data.fd = socketfd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &event) == -1)
    if (epollfd == -1)
    {
        std::cerr << "[E] epoll_ctl failed\n";
        return 1;
    }

    std::array<struct epoll_event, ::max_events> events;

    while (true)
    {
        auto n = epoll_wait(epollfd, events.data(), ::max_events, -1);
        for (int i = 0; i < n; ++i)
        {
            if (events[i].events & EPOLLERR ||
                events[i].events & EPOLLHUP ||
                !(events[i].events & EPOLLIN)) // error
            {
                std::cerr << "[E] epoll event error\n";
                close(events[i].data.fd);
            }
            else if (socketfd == events[i].data.fd) // new connection
            {
                while (::accept_connection(socketfd, event, epollfd)) {}
            }
            else // data to read
            {
                auto fd = events[i].data.fd;
                while (::read_data(fd)) {}
            }
        }
    }

    close(socketfd);
}