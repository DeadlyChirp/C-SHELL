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
// liste tous les jobs
int list_jobs(struct job *jobs) {
    if (!jobs) {
        printf("No jobs\n");
        return 1;
    }

    struct job *current_job = jobs;
    printf("ID\tStatus\t\tCommand\n");
    while (current_job != NULL) {
        char *status_str = "Unknown"; // Default status
        switch (current_job->etat) {
            case 1: // Running
                status_str = "Running";
                break;
            case 0: // Done
                status_str = "Done";
                break;
            case 2: // Stopped or other statuses can be handled here
                status_str = "Stopped";
                break;
        }
        
        printf("[%d]\t%s\t%s\n", current_job->id, status_str, current_job->command);
        current_job = current_job->next;
    }

    return 0;
}



// je pensais faire un job "racine" pour jsh mais trop relou wola

// ajoute un job à la liste des jobs
// ajoute un job à la liste des jobs
int init_job(char **commands, struct shell_info *shell, int bg, int status) {
    if (commands == NULL) {
        fprintf(stderr, "Error: commands is NULL\n");
        return EXIT_FAILURE;
    }

    if (shell == NULL) {
        fprintf(stderr, "Error: shell is NULL\n");
        return EXIT_FAILURE;
    }

    struct job *job = malloc(sizeof(struct job));
    if (job == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    // Assign the job ID correctly
    job->id = shell->last ? shell->last->id + 1 : 1;

    // Increment job count
    shell->nbr_jobs++;

    // Create a string to hold the full command with its arguments
    size_t command_length = 0;
    for (char **arg = commands; *arg != NULL; arg++) {
        command_length += strlen(*arg) + 1; // +1 for space or null terminator
    }

   char *full_command = malloc(command_length);
    if (full_command == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    full_command[0] = '\0'; // Initialize the string to empty
    for (char **arg = commands; *arg != NULL; arg++) {
        strcat(full_command, *arg);
        if (*(arg + 1) != NULL) {
            strcat(full_command, " "); // Add space between arguments
        }
    }
    job->command = full_command;
    job->etat = 1; // running
    job->bg = bg;
    job->next = NULL;

    if (shell->last != NULL) {
        shell->last->next = job;
    } else {
        shell->root = job;
    }
    shell->last = job;

    init_process(job, commands, shell, status);
    
    if (bg == 1) {
    bg_job(shell, job->id);
    // Print the job number and full command
    printf("[%d] %d Running %s\n", shell->nbr_jobs - 1, job->id, job->command);
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
        if (!process) {
            perror("malloc");
            return -1;
        }
        process->pid = pid;
        process->command = strdup(job->command);  // Use the full command from job
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

void add_job(struct shell_info *shell, char *command, pid_t pid, int is_bg) {
    struct job *new_job = malloc(sizeof(struct job));
    if (!new_job) {
        perror("Failed to allocate job");
        return;
    }

    new_job->command = strdup(command);
    new_job->pid = pid;
    new_job->bg = is_bg;
    new_job->etat = 1; // Running

    // Add to the front of the job list
    new_job->next = shell->root;
    shell->root = new_job;

    if (is_bg) {
        printf("[%d] %d Running %s\n", new_job->id, pid, command);
    }
}

void update_job_status(struct shell_info *shell, pid_t pid, int status) {
    struct job *job = shell->root;
    while (job != NULL) {
        if (job->pid == pid) {
            job->etat = status; // Update status to Done or Stopped
            break;
        }
        job = job->next;
    }
}
