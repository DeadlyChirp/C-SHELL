#ifndef SHELL_INFO_H
#define SHELL_INFO_H

#define NBR_JOBS 20
#define PATH_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define PROJECT_NAME_SIZE 100

#include "job.h"
#include "process.h"
#include <sys/types.h>
#include <string.h>

// structure 
struct shell_info {
    char cur_user[TOKEN_BUFSIZE]; // 
    char cur_dir[PATH_BUFSIZE];   // Current working directory
    char pw_dir[PATH_BUFSIZE];    // User's home directory
    struct job *jobs[NBR_JOBS + 1]; // Array of jobs
    int nbr_jobs;                   // Number of jobs
    char project_name[PROJECT_NAME_SIZE]; // Project name
};

#endif // SHELL_INFO_H
