#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MSGSIZE 16

int
main(int argc, char *argv[]) {
    sleep(10);
    // get the standard output info
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);
    // 文件描述符0，读标准输出；1，读标准输入；2，读错误信息。
    char *xargv[MAXARG];
    int xargc = 0;
    // copy the command and its args
    for (int i = 1; i < argc; ++i) xargv[xargc++] = argv[i];

    char *p = buf;
    for (int i = 0; i < MSGSIZE; ++i) {
        if (buf[i] == '\n') {
            int pid = fork();
            if (pid > 0) {
                p = &buf[i + 1];
                wait(0);
            } else {
                buf[i] = 0;
                xargv[xargc++] = p;
                xargv[xargc++] = 0;
                exec(xargv[0], xargv);
                exit(0);
            }
        }
    }
    exit(0);
}
