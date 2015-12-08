#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     512
#define SERV_PORT   1234    /* 服务器监听端口 */
#define MAX_EVENTS  10

/** 建立tcp套接字并监听指定的端口(服务端), 函数定义参见`api-demo/select-demo.c' **/
extern int tcp_server_socket(int port);
/** TCP服务器主函数 **/
int tcp_main(int client_fd);
/** 设置文件描述符标志 **/
int set_flag(int fd, int flag);

int main(int argc, char *argv[])
{
    int listen_fd, client_fd, epoll_fd;
    int i, nready;
    struct epoll_event ev, events[MAX_EVENTS];

    listen_fd = tcp_server_socket(SERV_PORT);
    
    if ((epoll_fd = epoll_create(MAX_EVENTS)) < 0) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        perror("epoll_ctl_add-listen_fd error");
        exit(EXIT_FAILURE);
    }
    
    for ( ; ; ) {
        if ((nready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) < 0) {
            if (errno == EINTR)
                continue;
            perror("epoll_wait error");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < nready; i++) {
            if (events[i].data.fd == listen_fd) {
                if ((client_fd = accept(listen_fd, NULL, NULL)) < 0) {
                    perror("accept error");
                    exit(EXIT_FAILURE);
                }
                
                ev.events = EPOLLIN | EPOLLET; /* 添加EPOLLET标志 */
                ev.data.fd = client_fd;
                if (set_flag(client_fd, O_NONBLOCK) < 0) { /* 设置为非阻塞 */
                    perror("set_flag error");
                    exit(EXIT_FAILURE);
                }
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                    perror("epoll_ctl_add-client_fd error");
                    exit(EXIT_FAILURE);
                }
            } else {
            /** 依据服务器所提供服务灵活改变结构 **/
                if (tcp_main(events[i].data.fd) == 0) {
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL) < 0) {
                        perror("epoll_ctl_del error");
                        exit(EXIT_FAILURE);
                    }
                    close(events[i].data.fd);
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}

static char buf[MAXLINE];

/** TCP服务器主函数: 返回值大于0表示服务未结束 **/
int tcp_main(int client_fd)
{
    int n;
    /* 保证将已发送的数据全部接收 */
    for ( ; ; ) {
        if ((n = read(client_fd, buf, MAXLINE)) < 0) {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN) /* 没有数据可读且文件描述符被设置为非阻塞时返回EAGAIN错误 */
                break;
            else {
                perror("read error");
                exit(EXIT_FAILURE);
            }
        }
        if (n == 0)
            return 0;
        buf[n] = '\0';
        printf("%s", buf);
    }
    return 1;
}

/** 设置文件描述符标志 **/
/** #!! 此函数会被其他程序重用 !!# **/
int set_flag(int fd, int flag)
{
    int value;
    if ((value = fcntl(fd, F_GETFL, 0)) < 0)
        return -1;
    value |= flag;
    if (fcntl(fd, F_SETFL, value) < 0)
        return -1;
    return value;
}
