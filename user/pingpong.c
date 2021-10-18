#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

#define STD_ERR 2
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if (argc != 1){
        fprintf(STD_ERR, "Usage:pingpong\n");
        exit(1);
    }

    int p1[2], p2[2];
    char buf[1];
    pipe(p1), pipe(p2);
    if (fork() == 0){
        close(p1[WRITE_END]);
        close(p2[READ_END]);
        if (read(p1[READ_END], buf, 1) == 1){
            printf("%d: received ping\n", getpid());
            close(p1[WRITE_END]);
        }
        write(p2[WRITE_END], buf, 1);
    }
    else {
        close(p2[WRITE_END]);
        close(p1[READ_END]);
        write(p1[WRITE_END], buf, 1);
        close(p1[WRITE_END]);
        if (read(p2[READ_END], buf, 1) == 1){
            printf("%d: received pong\n", getpid());
            close(p2[READ_END]);
        }
    }

    exit(0);
}
