#include <stdio.h>  // Pour les entrées/sorties standard
#include <stdlib.h> // Pour les fonctions de base (allocation mémoire, conversion de types, etc.)
#include <errno.h>  // Pour la gestion des erreurs
#include <unistd.h> // Pour les fonctions système (ex: sleep, fork, etc.)
#include "include/job.h"
#include "include/shell_info.h"

// utilitaires pour les jobs
struct job *find_job(struct shell_info *shell, int id)
{
    struct job *job = shell->root;

    while (job->id != id)
    {
        if (job->next != NULL)
        {
            job = job->next;
        }
        else
        {
            return NULL;
        }
    }
    return job;
}

// utilitaire pour les processus

int get_last_process_id(struct shell_info *shell)
{
    if (shell->last != NULL)
    {
        // obtenir le dernier processus
        struct process *tab_process = shell->last->processes;
        // obtenir le dernier id à partir du tableau de processus
        int i = 0;
        while (tab_process[i].pid != 0)
        {
            i++;
        }
        return tab_process[i - 1].pid;
    }
    else
    {
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

    printf("ID\tID\tetat\tcommande\n");
    do
    {
        printf("%d\t%d\t%d\t%s\n",
               jobs->id,
               jobs->id,
               jobs->etat,
               jobs->command);
    } while (jobs->next != NULL);

    return 0;
}

// je pensais faire un job "racine" pour jsh mais trop relou wola

// ajoute un job à la liste des jobs
int init_job(char **commands, struct shell_info *shell, int bg)
{
    if (commands == NULL) {
        fprintf(stderr, "Error: commands is NULL\n");
        return EXIT_FAILURE;
    }

    if (shell == NULL) {
        fprintf(stderr, "Error: shell is NULL\n");
        return EXIT_FAILURE;
    }

    struct job *job = malloc(sizeof(struct job));
    if (job == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    if (shell->last != NULL || shell->root != NULL) {    
        job->root = shell->root;
        job->next = NULL;
        job->id = shell->last->id + 1;
        job->etat = 1; // running
        job->bg = bg;
        shell->last->next = job;
        shell->last = job;
    } else {
        job->root = job;
        job->next = NULL;
        job->id = 1;
        job->etat = 1; // running
        job->bg = bg;
        shell->root = job;
        shell->last = job;
    }


    init_process(job, commands);

    return 0;
}


int init_process(struct job *job, char **argv)
{
    if (job == NULL) {
        fprintf(stderr, "Error: job is NULL\n");
        return -1;
    }

    if (argv == NULL) {
        fprintf(stderr, "Error: argv is NULL\n");
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        // Handle error
        perror("fork");
        return -1;
    }
    else if (pid == 0)
    {
        // This is the child process
        // Replace the process image with the new command
        if (execvp(argv[0], argv) < 0)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // This is the parent process
        // Add the new process to the job
        struct process *process = malloc(sizeof(struct process));
        if (!process)
        {
            perror("malloc");
            return -1;
        }
        process->pid = pid;
        process->command = argv[0];
        process->argv = argv;
        process->completed = 0;
        process->stopped = 0;
        process->next = job->processes;
        job->processes = process;
    }

    return 0;
}