#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>


//definition du type de processus
struct process {
    pid_t pid; // process id
    char *command; // commande du processus
    int argc; // nombre d'arguments
    char **argv; // tableau des arguments
    char *input_path; // chemin du fichier d'entrée
    char *output_path; // chemin du fichier de sortie
    int completed; // true(1) si le processus est terminé
    int stopped; // true(1) si le processus est stoppé
    struct process *next; //next pour piepline
};

#endif // PROCESS_H
