#ifndef JOB_H
#define JOB_H

#include "process.h"


// Definition of the job structure
struct job {
    int bg; // 0 = foreground, 1 = background 
    int id; //numero du job
    pid_t pgid; // process group id
    int etat; // done/running/stopped = 0/1/2
    char *command; // commande du job
    struct job *root; // pointer vers le processus racine de la liste des processus
    struct job *next; 
    int notified; // true (1)
    struct process *processes; // tableau des processus
};

#endif // JOB_H  