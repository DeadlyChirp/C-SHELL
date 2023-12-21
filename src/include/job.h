#ifndef JOB_H
#define JOB_H

#include "process.h"
#include "shell_info.h"
#include "command_exec.h"

// Definition of the job structure
struct job {
    int bg; // 0 = foreground, 1 = background 
    int id; //numero du job
    int etat; // done/running/stopped = 0/1/2
    char *command; // commande du job
    struct job *root; // pointer vers le processus racine de la liste des processus
    struct job *next; 
    int notified; // true (1)
    struct process *processes; // tableau des processus
};

int init_job(char **commands, struct shell_info *shell, int bg, int status);
int init_process(struct job *job, char **argv, struct shell_info *shell, int status);
struct job *find_job(struct shell_info *shell, int id);
int get_last_process_id(struct shell_info *shell);
int list_jobs(struct job *jobs);
void update_job_statuses(struct shell_info *shell);

#endif // JOB_H  