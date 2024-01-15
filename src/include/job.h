#ifndef JOB_H
#define JOB_H

#include "process.h"
#include "shell_info.h"
#include "command_exec.h"

// Definition of the job structure
struct job {
     int bg;          // 0 = foreground, 1 = background
    int id;          // job number
    pid_t pid;       // process ID
    int etat;        // done/running/stopped = 0/1/2
    char *command;   // command of the job
    struct job *next; // next job in the list
    int notified;    // true (1) if job status has been reported
    struct process *processes; // list of processes in the job
};


int init_job(char **commands, struct shell_info *shell, int bg, int status);
int init_process(struct job *job, char **argv, struct shell_info *shell, int status);
struct job *find_job(struct shell_info *shell, int id);
int get_last_process_id(struct shell_info *shell);
int list_jobs(struct job *jobs);
void update_job_statuses(struct shell_info *shell);
void sigchld_handler(int signo);
void sigterm_handler(int signo);
kill_job(struct shell_info *shell, int id);
#endif // JOB_H  