#ifndef JOB_H
#define JOB_H

#include "process.h"

struct job {
    int id;
    struct process *root;
    char *command;
    pid_t pgid;
    int mode;
};

#endif // JOB_H
