#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "process.h"

int command(struct process_s *, const char *);
int command(struct process_s *ps, const char *buf){
    process_write(ps, buf, strlen(buf));
    return 0;
}

int main(int argc, const char *argv[]){
    char buf[BUFSIZ];
    struct process_s *ps;
    FILE *fp;

    if(argc < 3){
        fprintf(stderr, "Usage: %s SCRIPT_FILE COMMAND ARGS\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "r");
    if(fp == NULL){
        perror("main:fopen");
        return 1;
    }
    ps = process_init();
    ps = process_set(ps, argv[2], &argv[3]);
    process_print(ps);
    process_start(ps);
    while(fgets(buf, BUFSIZ, fp) != NULL){
        if(strncmp(buf, "\n", 1) != 0){
            if(buf[strlen(buf) - 1] == '\n'){
                buf[strlen(buf) - 1] = '\0';
            }
        }
        command(ps, buf);
    }
    process_wait(ps);
    return 0;
}
