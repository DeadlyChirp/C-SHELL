#ifndef JOB_H
#define JOB_H

#include "process.h"


// Definition of the job structure
struct job {
    int id;
    struct process *root; // pointer vers le processus racine de la liste des processus
    char *command; // commande du job
    pid_t pgid; // group id du job
    int mode; // mode du job
};

#endif // JOB_H
