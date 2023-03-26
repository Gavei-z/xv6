nclude "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 36

void prime(int read_pipe, int write_pipe) {
    char buf[MSGSIZE];
    int res = 0;
    read(read_pipe, buf, MSGSIZE);
    for (int i = 0; i < MSGSIZE; ++ i)
        if (buf[i] == '1') {
            res = i;
            break;
        }
    if (res == 0) exit(0);
    printf("prime %d\n", res);
    buf[res] = '0';
    for (int i = res; i < MSGSIZE; ++ i) {
        if (i % res == 0) buf[i] = '0';
    }

    int pid = fork();
    if (pid > 0)
        write(write_pipe, buf, MSGSIZE);
    if (pid == 0)
        prime(read_pipe, write_pipe);
}

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    char buf[MSGSIZE];
    for (int i = 2; i < MSGSIZE; ++i)
        buf[i] = '1';
    int pid = fork();
    if (pid > 0) {
        buf[0] = '0';
        buf[1] = '0';
        write(fd[1], buf, MSGSIZE);
        wait(0);
    }
    if (pid == 0) {
        prime(fd[0], fd[1]);
        wait(0);
    }
    exit(0);
}
