#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include "prompt.h"
#include "shell_info.h"



extern struct shell_info *shell;

// fonction pour tronquer le chemin du répertoire si trop long
char *truncate_dir_path(char *dir, int max_length) {
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
//fonction pour mettre à jour les informations sur le répertoire de travail actuel
void update_cwd_info() {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
    }
}

// fonction pour afficher le prompt du shell
void print_prompt() {
    update_cwd_info();  // Update the current working directory information

    char *truncated_dir = truncate_dir_path(shell->cur_dir, 20);  // Truncate the directory path if too long

    // Print the prompt with either the full or truncated directory path
    printf("\033[32m" "\001" "[%d]" "\002" "\033[33m" "\001" " %s" "\002" "\033[00m" "$ ", 
           shell->nbr_jobs, truncated_dir);

    if (truncated_dir != shell->cur_dir) {
        free(truncated_dir);
    }
}
