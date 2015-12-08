#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int nchild;
pid_t *pids;

/** 子进程主函数 **/
void child_main();
/** 生成一个子进程, 返回进程ID **/
pid_t make_child();
/** SIGCHLD信号处理函数 **/
void sig_child(int sig);
/** SIGINT信号处理函数 **/
void sig_int(int sig);

int main(int argc, char *argv[])
{
    int i;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <slave-num>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    nchild = atoi(argv[1]);
    if (nchild < 1) {
        fprintf(stderr, "The slave num is not enough.\n");
        exit(EXIT_FAILURE);
    }
    
    /** 注册信号处理函数 **/
    if (signal(SIGCHLD, sig_child) == SIG_ERR) {
        perror("signal SIGCHLD error");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        perror("signal SIGINT error");
        exit(EXIT_FAILURE);
    }
    
    /** 分配进程 **/
    pids = malloc(sizeof(pid_t) * nchild);
    for (i = 0; i < nchild; i++) {
        pids[i] = make_child();
    }
    
    /** 进入"死循环" **/
    for (; ;)
        pause();
    exit(EXIT_SUCCESS);
}

/** 子进程 **/
void child_main()
{
    printf("child %ld started!\n", (long)getpid());
    if (execlp("./child", "child", (char *)0) < 0)
        exit(EXIT_FAILURE);
}

/** 生成一个子进程, 返回进程ID **/
pid_t make_child()
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { /* 父进程 */
        return pid;
    }
    /* 子进程 */
    child_main();
}

/** SIGCHLD信号处理函数: 当一个子进程结束后, 再次生成一个子进程 **/
void sig_child(int sig)
{
    pid_t pid;
    int i, status;
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("process %ld is finished, status %d.\n", (long)pid, status);
        for (i = 0; i < nchild; i++) {
            if (pid == pids[i]) {
                pids[i] = make_child();
                break;
            }
        }
    }
}

/** SIGINT信号处理函数: 此进程被中断时, 给所有子进程发送SIGTERM信号, 杀死所有子进程 **/
void sig_int(int sig)
{
    int i;
    time_t t;
    
    /** 恢复SIGCHLD的信号处理函数, 防止杀死后重启 **/
    if (signal(SIGCHLD, SIG_DFL) == SIG_ERR) {
        perror("signal SIGCHLD [default] error");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < nchild; i++) {
        kill(pids[i], SIGTERM);
        printf("child terminated: %ld\n", (long)pids[i]);
    }
    time(&t);
    printf("process terminated@ %s", ctime(&t));
    exit(EXIT_SUCCESS);
}
