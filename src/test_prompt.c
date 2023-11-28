#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include "include/prompt.h"
#include "include/shell_info.h"

struct shell_info *shell;

void initialize_shell() {
    shell = malloc(sizeof(struct shell_info));
    if (!shell) {
        fprintf(stderr, "Failed to allocate memory for shell\n");
        exit(EXIT_FAILURE);
    }

    // Current user
    getlogin_r(shell->cur_user, sizeof(shell->cur_user));

    // User's working directory
    if (getcwd(shell->cur_dir, sizeof(shell->cur_dir)) == NULL) {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

    // Test for the number of jobs
    shell->nbr_jobs = 0;
    strcpy(shell->project_name, "YourProjectName"); 
}

void simulate_shell_behavior() {
    char input[100];
    printf("Enter 'cd <path>' to change directory, 'jobs <number>' to set jobs, or 'exit' to quit.\n");

    while (1) {
        // print_prompt();
        fgets(input, sizeof(input), stdin);
        
        // Removing trailing newline character
        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "cd ", 3) == 0) {
            // Change directory
            if (chdir(input + 3) != 0) {
                perror("chdir() failed");
            } else {
                getcwd(shell->cur_dir, sizeof(shell->cur_dir));
            }
        } else if (strncmp(input, "jobs ", 5) == 0) {
            // Change number of jobs
            shell->nbr_jobs = atoi(input + 5);
        } else if (strcmp(input, "exit") == 0) {
            break;
        }
    }
}

