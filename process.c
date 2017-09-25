#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "process.h"

struct process_s *process_init(void){
    struct process_s *ps;
    ps = (struct process_s *)malloc(sizeof(struct process_s));
    ps->command = NULL;
    ps->args = NULL;
    ps->pid = 0;
    ps->fd[0] = 0;
    ps->fd[1] = 0;
    ps->status = -1;
    return ps;
}

struct process_s *process_set(struct process_s *ps, const char *command, char const *args[]){
    size_t i;
    char const **cptr;
    char **ptr;

    if(command != NULL){
        if(ps->command != NULL) free(ps->command);
        ps->command = strdup(command);
    }
    if(args != NULL){
        if(ps->args != NULL){
            ptr = ps->args;
            while(*ptr != NULL){
                free(*ptr);
                *ptr++ = NULL;
            }
        }
        i = 1, cptr = args;
        while(*cptr != NULL) i++, cptr++;
        ps->args = (char **)malloc(sizeof(char *) * i);
        //ps->args = (char **)malloc(sizeof(char *) * n + 1);
        i = 0, cptr = args;
        while(*cptr != NULL){
            ps->args[i++] = strdup(*cptr++);
        }
        ps->args[i] = NULL;
    }
    return ps;
}

void process_destroy(struct process_s *ps){
    char **ptr;
    if(ps->command != NULL) free(ps->command);
    if(ps->args != NULL){
        ptr = ps->args;
        while(*ptr != NULL){
            free(*ptr);
            *ptr++ = NULL;
        }
        free(ps->args);
    }
    free(ps);
}

struct process_s *process_print(struct process_s *ps){
    char **ptr;
    printf("process {\n");
    printf("    command: \"%s\"\n", ps->command);
    printf("    args: ");
    for(ptr = ps->args; *ptr != NULL; ptr++) printf(" \"%s\"", *ptr);
    printf("\n");
    printf("    pid: %d\n", ps->pid);
    printf("    fd: [%d, %d]\n", ps->fd[0], ps->fd[1]);
    printf("    status: %d\n", ps->status);
    printf("}\n");
    return ps;
}

int process_start(struct process_s *ps){
    if(pipe(ps->fd) < 0){
        perror("process_start:pipe");
        return 1;
    }
    ps->pid = fork();
    if(ps->pid == -1){
        perror("process_start:fork");
        return 1;
    }else if(ps->pid == 0){
        dup2(ps->fd[1], fileno(stdin));
        //dup2(ps->fd[1], fileno(stdout));
        close(ps->fd[0]);
        execvp(ps->command, ps->args);
        perror("process_start:execvp");
        return 1;
    }
    close(ps->fd[1]);
    return 0;
}
int process_wait(struct process_s *ps){
    waitpid(ps->pid, &ps->status, 0);
    return ps->status;
}

ssize_t process_write(struct process_s *ps, const char *buf, size_t n){
    ssize_t ret;
    ret = write(ps->fd[0], buf, n);
    if(ret == -1) perror("process_write:write");
    return ret;
}
ssize_t process_read(struct process_s *ps, char *buf, size_t n){
    ssize_t ret;
    ret = read(ps->fd[0], buf, n);
    if(ret == -1) perror("process_read:read");
    return ret;
}
