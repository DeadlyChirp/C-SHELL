#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include "prompt.h"
#include "shell_info.h"
// Assume this contains the struct shell_info and other relevant declarations

extern struct shell_info *shell;

// Helper function to truncate directory path if too long
char *truncate_dir(char *dir, int max_length) {
    int dir_len = strlen(dir);
    if (dir_len <= max_length) {
        return dir;
    }

    char *truncated = malloc(max_length + 1);
    if (!truncated) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    sprintf(truncated, "...%s", dir + dir_len - max_length + 3);
    return truncated;
}

// Function to update current working directory in shell_info
void update_cwd_info() {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
    }
}

// Function to print the shell prompt
void mysh_print_prompt() {
    update_cwd_info(); // Update the current working directory info

    char *truncated_dir = truncate_dir(shell->cur_dir, 20); // Truncate if longer than 20 characters

    // Create and print the prompt
    printf(COLOR_CYAN "[%d]" COLOR_NONE " %s in " COLOR_YELLOW "%s" COLOR_NONE "\n",
           shell->nr_jobs, shell->cur_user, truncated_dir);

    if (truncated_dir != shell->cur_dir) {
        free(truncated_dir);
    }

    printf(COLOR_RED "mysh>" COLOR_NONE " ");

}
