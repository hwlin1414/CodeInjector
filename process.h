#include <sys/types.h>
#include <unistd.h>

struct process_s {
    char *command;
    char **args;
    pid_t pid;
    int fd[2];
    int status;
};

struct process_s *process_init(void);
void process_destroy(struct process_s *);

struct process_s *process_set(struct process_s *, const char *, char const *[]);
struct process_s *process_print(struct process_s *);

int process_start(struct process_s *);
int process_wait(struct process_s *);

ssize_t process_write(struct process_s *, const char *, size_t);
ssize_t process_read(struct process_s *, char *, size_t);
