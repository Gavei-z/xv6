#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

#define MSGSIZE 16
char buf[MSGSIZE];

// 管道只是单向通信，可以一个进程写，一个进程读，它可以被视为一个虚拟文件
// 若管道是空的，一个进程在读的时候会被挂起，直到有内容被写入管道
// 创建管道，int fd[2]; pipe[fd]; 从fd[0]里面读数据，fd[1]里面写数据。

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: ping pong\n");
        exit(1);
    }

    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(2, "Pipe error, exit\n");
        exit(1);
    }
    int pid = fork();
    if (pid > 0) {
        write(fd[1], "ping", MSGSIZE);
        wait(NULL);
        read(fd[0], buf, MSGSIZE);
        printf("%d: received %s\n", getpid(), buf);
    } else {
        read(fd[0], buf, MSGSIZE);
        printf("%d: received %s\n", getpid(), buf);
        write(fd[1],"pong",MSGSIZE);
    }
    exit(0);
}
