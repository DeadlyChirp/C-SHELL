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

get_last_job_id(struct shell_info *shell)
{
    if (shell->last != NULL)
    {
        return shell->last->id;
    }
    else
    {
        return 0;
    }
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
int init_job(char **commands, struct shell_info *shell, int bg, int status)
{
    if (commands == NULL)
    {
        fprintf(stderr, "Error: commands is NULL\n");
        return EXIT_FAILURE;
    }

    if (shell == NULL)
    {
        fprintf(stderr, "Error: shell is NULL\n");
        return EXIT_FAILURE;
    }

    struct job *job = malloc(sizeof(struct job));
    if (job == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    if (shell->last != NULL || shell->root != NULL)
    {
        job->root = shell->root;
        job->next = NULL;
        job->id = get_last_job_id(shell) + 1;
        job->etat = 1; // running
        job->bg = bg;
        shell->last->next = job;
        shell->last = job;
        shell->nbr_jobs++;
    }
    else
    {
        job->root = job;
        job->next = NULL;
        job->id = 1;
        job->etat = 1; // running
        job->bg = bg;
        shell->root = job;
        shell->last = job;
        shell->nbr_jobs = 1;
    }

    init_process(job, commands, shell, status);

    if (bg == 1)
    {
        bg_job(shell, job->id);
    }

    return 0;
}

int init_process(struct job *job, char **argv, struct shell_info *shell, int status )
{
    if (job == NULL)
    {
        fprintf(stderr, "Error: job is NULL\n");
        return -1;
    }

    if (argv == NULL)
    {
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
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            shell->dernier_statut = WEXITSTATUS(status); // Prendre le statut de sortie de l'enfant
        }
        else if (WIFSIGNALED(status))
        {
            shell->dernier_statut = WTERMSIG(status); // commande tuée par un signal
        }
        else
        {
            shell->dernier_statut = EXIT_FAILURE; // Erreur inconnue
        }

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

// mettrre le job en avant plan
int fg_job(struct shell_info *shell, int job_id)
{
    struct job *job = find_job(shell, job_id);
    if (!job)
    {
        fprintf(stderr, "Job %d non trouve\n", job_id);
        return -1;
    }

    // if (kill(-job->pgid, SIGCONT) < 0) {
    //     perror("Error sending SIGCONT");
    //     return -1;
    // }

    // wait_for_job(shell, job_id);

    return 0;
}

// mettre le job en arrière plan
int bg_job(struct shell_info *shell, int job_id)
{
    struct job *job = find_job(shell, job_id);
    if (!job)
    {
        fprintf(stderr, "Job %d not found\n", job_id);
        return -1;
    }

    // if (kill(-job->pgid, SIGCONT) < 0) {
    //     perror("Error sending SIGCONT");
    //     return -1;
    // }

    job->bg = 1;

    return 0;
}

int kill_job(struct shell_info *shell, int job_id)
{
    struct job *job = find_job(shell, job_id);
    if (!job)
    {
        fprintf(stderr, "Job %d not found\n", job_id);
        return -1;
    }

    if (kill(-job->id, SIGKILL) < 0)
    {
        perror("Error sending SIGKILL");
        return -1;
    }

    return 0;
}

int exit_all_jobs(struct shell_info *shell)
{
    if (shell->nbr_jobs > 0)
    {
        struct job *job = shell->root;
        while (job->next != NULL)
        {
            kill_job(shell, job->id);
            job = job->next;
        }
        shell->nbr_jobs = 0;
        return 0;
    }
    else
    {
        printf("Pas de jobs\n");
        return 1;
    }
}