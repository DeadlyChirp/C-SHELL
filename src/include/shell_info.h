#ifndef SHELL_INFO_H
#define SHELL_INFO_H

#include "job.h"

#define NR_JOBS 20
#define PATH_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
struct shell_info {
    char cur_user[TOKEN_BUFSIZE];
    char cur_dir[PATH_BUFSIZE];
    char pw_dir[PATH_BUFSIZE];
    struct job *jobs[NR_JOBS + 1];
    int nr_jobs;  // Keep track of the number of jobs
};

#endif // SHELL_INFO_H
