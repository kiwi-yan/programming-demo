#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXLINE     512
#define SERV_PORT   1234    /* 服务器监听端口 */

/** 建立TCP套接字并监听指定的端口(服务端), 函数定义参见`api-demo/select-demo.c' **/
extern int tcp_server_socket(int port);
/** TCP服务器主函数 **/
int tcp_main(int client_fd);

int main(int argc, char *argv[])
{
    int listen_fd, client_fd;
    struct pollfd client[_POSIX_OPEN_MAX];      /* pollfd结构的描述符集合 */
    int i, max_i, nready;
    
    listen_fd = tcp_server_socket(SERV_PORT);

    /** 初始化pollfd结构数组 **/
    client[0].fd = listen_fd;
    client[0].events = POLLIN;
    for (i = 1; i < _POSIX_OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    max_i = 0;
    
    /** 简单的单进程poll多路复用并发服务 **/
    for ( ; ; ) {
        /** 获取可读的描述符 **/
        if ((nready = poll(client, max_i + 1, -1)) < 0) { 
            perror("poll error");
            exit(EXIT_FAILURE);
        } /* glibc中未提供宏INFTIM, 最后一个参数用-1代替 */

        if (client[0].revents & POLLIN) {   /* 有新连接 */
            /** 接受来自客户端的连接 **/
            if ((client_fd = accept(listen_fd, NULL, NULL)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            /** 将新连接加入客户表 **/
            for (i = 1; i < _POSIX_OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = client_fd;
                    break;
                }
            }
            if (i == _POSIX_OPEN_MAX) {      /* 连接数太多, 服务器宣告崩溃 */
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);
            }

            client[i].events = POLLIN;
            if (i > max_i)
                max_i = i;

            if (--nready <= 0)      /* 没有可读描述符 */
                continue;
        }

        for (i = 1; i <= max_i; i++) {   /* 检查所有客户描述符 */
            if (client[i].fd < 0)
                continue;
            if (client[i].revents & (POLLIN | POLLERR)) { /* OK */
            /** 依据服务器所提供服务灵活改变结构 **/
                if (tcp_main(client[i].fd) == 0) {
                    /** 一个客户服务结束应采取的操作 **/
                    close(client[i].fd);
                    if (max_i == i)
                        --max_i;
                    client[i].fd = -1;
                }
                if (--nready <= 0)   /* 没有可读描述符 */
                    break;
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
        if (errno == ECONNRESET)
            return 0;
        perror("read error");
        exit(EXIT_FAILURE);
    }
    if (n == 0)
        return 0;
    buf[n] = '\0';
    printf("%s", buf);
    return 1;
}
