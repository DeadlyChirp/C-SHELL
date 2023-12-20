#ifndef SHELL_INFO_H
#define SHELL_INFO_H

#define NBR_JOBS 20
#define PATH_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define USERNAME_BUFSIZE 64 
#define PROJECT_NAME_SIZE 100

#include "job.h"
#include "process.h"
#include <sys/types.h>
#include <string.h>

// structure 
struct shell_info {
    char cur_user[USERNAME_BUFSIZE]; // utilisateur courant
    char cur_dir[PATH_BUFSIZE];   // repertoire courant
    char pw_dir[PATH_BUFSIZE];    // repertoir de travail
    char prev_dir[PATH_BUFSIZE]; // repertoire precedent
    struct job *root; // premier job de la liste
    struct job *last; // dernier job de la liste
    int nbr_jobs;                   // nombre de jobs
    char project_name[PROJECT_NAME_SIZE]; // Nom du projet
    int dernier_statut; // dernier statut
};

// In shell_info.h
extern struct shell_info *shell;


#endif // SHELL_INFO_H
