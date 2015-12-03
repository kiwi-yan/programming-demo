#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAXLINE		512
#define SERV_PORT	1234	/* 服务器监听端口 */
#define BACKLOG		12		/* 积压值 */

/** 设置SIGCHLD信号处理函数 **/
void wait_child(int sig) {
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main()
{
	int listen_fd, client_fd;
	struct sockaddr_in serv_addr;
	pid_t child_pid;
	
	/** 建立TCP套接字 **/
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	/** 设置地址相关信息 **/
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);
	
	/** 绑定地址 **/
	if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	
	/** 监听套接字 **/
	if (listen(listen_fd, BACKLOG) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
	
	/** 注册信号处理函数 **/
	if (signal(SIGCHLD, wait_child) == SIG_ERR) {
		perror("signal error");
		exit(EXIT_FAILURE);
	}
	
	/** 简单的多进程并发服务 **/
	for ( ; ; ) {
		/** 接受来自客户端的连接 **/
		if ((client_fd = accept(listen_fd, NULL, NULL)) < 0) {
			perror("accept error");
			exit(EXIT_FAILURE);
		}
		
		if ((child_pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		} else if (child_pid == 0) {		/* 子进程 */
			close(listen_fd);  /** 关闭无用的文件描述符 **/
			tcp_main(client_fd);
			close(client_fd);
			exit(EXIT_SUCCESS);
		}
		/* 父进程 */
		close(client_fd);
	}
	exit(EXIT_SUCCESS);
}

void tcp_main(int client_fd)
{
	char buf[MAXLINE];
	strncpy(buf, "The connection will be closed...\n", MAXLINE);
	if (write(client_fd, buf, MAXLINE) < 0) {
		exit(EXIT_FAILURE);
	}
}
