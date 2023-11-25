#ifndef SHELL_INFO_H
#define SHELL_INFO_H
#define NBR_JOBS 20
#define PATH_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#include "job.h"
#include "process.h"
#include <sys/types.h>
#include <string.h>
#include "prompt.h"

// Definition of the shell_info structure
struct shell_info {
    char cur_user[TOKEN_BUFSIZE]; // utilisateur courant
    char cur_dir[PATH_BUFSIZE]; // répertoire de travail courant
    char pw_dir[PATH_BUFSIZE]; // répertoire de travail de l'utilisateur
    struct job *jobs[NBR_JOBS + 1]; // tableau des jobs
    int nbr_jobs;  // nombre de jobs
    char project_name[100];
};

#endif // SHELL_INFO_H
