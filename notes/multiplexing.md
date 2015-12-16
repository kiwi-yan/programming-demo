# I/O多路复用 (multiplexing)

I/O多路复用, 通常又称作I/O事件驱动或I/O多路转接. 用于同时监听多个文件描述符, 返回满足条件的描述符.
I/O多路复用的系统调用主要有(后面的文字叙述是Linux Manual Page对它们的描述)

- select: 同步I/O多路复用
- poll: 等待描述符上的事件
- epoll: I/O事件通知机制

select和poll是传统的UNIX上的I/O多路复用系统调用, 已经被POSIX采纳.

epoll仅用于Linux系统(kernel版本2.5.44以后).

在介绍这些函数用法之前, 先介绍以下几个概念:
1. 描述符可读
2. 描述符可写
3. 描述符异常

## select

select函数原型如下:

    #include <sys/select.h>
    int select(int nfds, fd_set *readfds, fd_set *writefds,
                fd_set *exceptfds, struct timeval *timeout);

同时定义了以下几个方法, 可用于操作fd_set:

    void FD_CLR(int fd, fd_set *set);
    int  FD_ISSET(int fd, fd_set *set);
    void FD_SET(int fd, fd_set *set);
    void FD_ZERO(fd_set *set);


## poll

poll函数原型如下:

    #include <poll.h>
    int poll(struct pollfd *fds, nfds_t nfds, int timeout);


<!-- 在其他类UNIX系统上也有对应的系统调用, 如/dev/poll接口(Solaris), kqueue系统调用(FreeBSD). -->
## epoll

- epoll_create可以用来创建一个epoll实例并返回一个fd.
- 使用epoll_ctl来注册感兴趣的fd, 这些fd有时候被称为epoll set.
- epoll_wait用来等待事件的发生. (blocking).

关于水平触发与边缘触发:
对于等待读事件的fd, 边缘触发模式下每次epoll_wait阻塞返回时应该将fd可读内容全部处理, 第二次阻塞前会清空可读内容. 水平触发模式下会再次返回. 也就是说, 边缘触发模式仅当被监视的文件描述符发生更改时才提供事件.
边缘触发需设置EPOLLET标志, 同时fd应该设置为非阻塞的.


## 参考资料
- UNIX Network Programming, 3e
- Linux Manual Page
