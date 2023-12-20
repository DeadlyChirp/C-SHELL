#include <stdio.h>  // Pour les entrées/sorties standard
#include <stdlib.h> // Pour les fonctions de base (allocation mémoire, conversion de types, etc.)
#include <errno.h>  // Pour la gestion des erreurs
#include <unistd.h> // Pour les fonctions système (ex: sleep, fork, etc.)
#include "include/job.h"
#include "include/shell_info.h"

int get_last_process_id(struct shell_info *shell){
    if(shell -> last != NULL){
        //obtenir le dernier processus
        struct process *tab_process = shell->last->processes;
        //obtenir le dernier id à partir du tableau de processus
        int i = 0;
        while(tab_process[i].pid != 0){
            i++;
        }
        return tab_process[i-1].pid;
    }
    else{
        return 0;
    }
}

// liste tous les jobs
int list_jobs(struct job *jobs)
{
    if (jobs->root != NULL)
    {
        jobs = jobs->root;
    }
    else
    {
        printf("Pas de jobs root\n");
        return 1;
    }

    printf("ID\tPGID\tetat\tcommande\n");
    do
    {
        printf("%d\t%d\t%d\t%s\n",
               jobs->id,
               jobs->pgid,
               jobs->etat,
               jobs->command);
    } while (jobs->next != NULL);

    return 0;
}

//je pensais faire un job "racine" pour jsh mais trop relou wola 

// ajoute un job à la liste des jobs
int init_job(struct process *processes, struct shell_info *shell, int array_size, char *command, int bg){
    struct job *job = malloc(sizeof(struct job) + array_size * sizeof(struct process));
    if (job == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }
    // copilot a mit ça là mais je vois pas
    // memset(job, 0, sizeof(struct job));
    memcpy(job->processes, processes, sizeof(struct process) * array_size);
    job->root = shell->root;
    job->next = NULL;
    job->id = shell->last->id + 1;
    //taille tab process last+1
    job->pgid = shell->last->processes[array_size].pid + 1;
    job->etat = 1; //running
    job->command = command;
    job->bg = bg;
    shell->last->next = job;    
    shell->last = job;
    
    return 0;
}

struct process *init_process(char *command, int argc, char **argv, char *iputPath, char *outputPath){
    return 0;
}