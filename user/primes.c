#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

#define STD_ERR 2
#define READ_END 0
#define WRITE_END 1

void work(int pp[]){
    close(pp[WRITE_END]);
    int i;
    if (read(pp[READ_END], &i, 4) == 0){
        close(pp[READ_END]);
        exit(0);
    }
    printf("prime %d\n",i);
    int pc[2], num; //pipe--child
    pipe(pc);
    if (fork() == 0){
        close(pp[READ_END]);
        close(pc[WRITE_END]);
        work(pc);
    }
    else {
        close(pc[READ_END]);
        while (read(pp[READ_END], &num, 4) != 0){
            if (num % i != 0){
                write(pc[WRITE_END], &num, 4);
            }
        }
        close(pc[WRITE_END]);
        close(pp[READ_END]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[]){
    if (argc != 1){
        fprintf(STD_ERR, "Usage:primes\n");
        exit(1);
    }

    int pp[2];
    pipe(pp);
    if (fork() == 0){
        work(pp);
    }
    else {
        close(pp[READ_END]);
        for (int i = 2; i <= 35; i++){
            write(pp[WRITE_END], &i, 4);
        }
        close(pp[WRITE_END]);
        wait(0);
    }

    exit(0);
}
