#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>


//definition du type de processus
struct process {
    char *command; // commande du processus
    int argc; // nombre d'arguments
    char **argv; // tableau des arguments
    char *input_path; // chemin du fichier d'entrée
    char *output_path; // chemin du fichier de sortie
    pid_t pid; // process id
    int type; // type de processus
    int status; // status du processus
    struct process *next; // pointeur vers le processus suivant
};

#endif // PROCESS_H
