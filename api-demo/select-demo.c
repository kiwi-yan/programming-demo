#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE     512
#define SERV_PORT   1234    /* 服务器监听端口 */
#define BACKLOG     12      /* 积压值 */

/** 建立TCP套接字并监听指定的端口(服务端) **/
int tcp_server_socket(int port);
/** TCP服务器主函数 **/
int tcp_main(int client_fd);

int main(int argc, char *argv[])
{
    int listen_fd, client_fd;
    int client[FD_SETSIZE], max_fd, max_i;  /* 客户连接描述符 */
    fd_set allset, rset;                    /* 所有描述符集合与可读描述符集合 */
    int i, nready;
    
    listen_fd = tcp_server_socket(SERV_PORT);

    /** 初始化描述符集合 **/
    max_fd = listen_fd;
    max_i = -1;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listen_fd, &allset);
    
    /** 简单的单进程select多路复用并发服务 **/
    for ( ; ; ) {
        rset = allset;   /* select会修改传入的描述符集合, 注意备份 */
        /** 获取可读的描述符 **/
        if ((nready = select(max_fd+1, &rset, NULL, NULL, NULL)) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(listen_fd, &rset)) {   /* 有新连接 */
            /** 接受来自客户端的连接 **/
            if ((client_fd = accept(listen_fd, NULL, NULL)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            /** 将新连接加入客户表 **/
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = client_fd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {      /* 连接数太多, 服务器宣告崩溃 */
                fprintf(stderr, "too many clients\n");
                exit(EXIT_FAILURE);
            }

            FD_SET(client_fd, &allset);
            if (client_fd > max_fd)
                max_fd = client_fd;
            if (i > max_i)
                max_i = i;

            if (--nready <= 0)      /* 没有可读描述符 */
                continue;
        }

        for (i = 0; i <= max_i; i++) {   /* 检查所有客户描述符 */
            if (client[i] < 0)
                continue;
            if (FD_ISSET(client[i], &rset)) { /* OK */
            /** 依据服务器所提供服务灵活改变结构 **/
                if (tcp_main(client[i]) == 0) {
                    /** 一个客户服务结束应采取的操作 **/
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                    if (client[i] == max_fd)
                        --max_fd;
                    if (max_i == i)
                        --max_i;
                    client[i] = -1;
                }
                if (--nready <= 0)   /* 没有可读描述符 */
                    break;
            }
        }
    }
    exit(EXIT_SUCCESS);
}

/** 建立tcp套接字并监听指定的端口（服务端） **/
/** #!! 此函数会被其他程序重用 !!# **/
int tcp_server_socket(int port)
{
    int sock_fd, opt;
    struct sockaddr_in serv_addr;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    /** 设置SO_REUSEADDR选项 **/
    opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0) {
        perror("socket option SO_REUSEADDR error");
        exit(EXIT_FAILURE);
    }
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    
    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    
    if (listen(sock_fd, BACKLOG) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    
    return sock_fd;
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
