#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

struct process {
    char *command;
    int argc;
    char **argv;
    char *input_path;
    char *output_path;
    pid_t pid;
    int type;
    int status;
    struct process *next;
};

#endif // PROCESS_H
