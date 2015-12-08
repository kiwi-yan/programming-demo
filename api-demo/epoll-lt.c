#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     512
#define SERV_PORT   1234    /* 服务器监听端口 */
#define MAX_EVENTS  10

/** 建立TCP套接字并监听指定的端口(服务端), 函数定义参见`api-demo/select-demo.c' **/
extern int tcp_server_socket(int port);
/** TCP服务器主函数 **/
int tcp_main(int client_fd);

int main(int argc, char *argv[])
{
    int listen_fd, client_fd, epoll_fd;
    int i, nready;
    struct epoll_event ev, events[MAX_EVENTS];

    listen_fd = tcp_server_socket(SERV_PORT);
    
    /** 创建epoll实例，并将listen_fd加入epoll集合 **/
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
    
    /** 简单的单进程epoll事件驱动并发服务 **/
    for ( ; ; ) {
        /** 获取可读的描述符 **/
        if ((nready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) < 0) {
            if (errno == EINTR) /* epoll_wait可能会被信号中断 */
                continue;
            perror("epoll_wait error");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < nready; i++) {
            if (events[i].data.fd == listen_fd) { /* 有新连接 */
                if ((client_fd = accept(listen_fd, NULL, NULL)) < 0) {
                    perror("accept error");
                    exit(EXIT_FAILURE);
                }
                /* 将新连接加入epoll集合 */
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                    perror("epoll_ctl_add-client_fd error");
                    exit(EXIT_FAILURE);
                }
            } else { /* 处理新的客户端请求 */
            /** 依据服务器所提供服务灵活改变结构 **/
                if (tcp_main(events[i].data.fd) == 0) {
                    /** 一个客户服务结束应采取的操作 **/
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL) < 0) {
                        perror("epoll_ctl_del error");
                        exit(EXIT_FAILURE);
                    }   /* 删除一个描述符时最后一个参数是被忽略的, 但在Linux 2.6.9之前, 这个参数不能为NULL */
                    close(events[i].data.fd);
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}

/** TCP服务器主函数: 返回值大于0表示服务未结束 **/
int tcp_main(int client_fd)
{
    char buf[MAXLINE];
    int n = read(client_fd, buf, MAXLINE);
    
    if (n < 0) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    if (n == 0)
        return 0;
    buf[n] = '\0';
    printf("%s", buf);
    return 1;
}
