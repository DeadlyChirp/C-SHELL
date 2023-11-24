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

//fonction pour mettre à jour les informations sur le répertoire de travail actuel
void update_cwd_info() {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
    }
}

// fonction pour afficher le prompt du shell
void mysh_print_prompt() {
    update_cwd_info(); //fonction pour mettre à jour les informations sur le répertoire de travail actuel

    char *truncated_dir = truncate_dir(shell->cur_dir, 20); // fonction pour tronquer le chemin du répertoire si trop long

    // création du prompt 
    printf(COLOR_CYAN "[%d]" COLOR_NONE " %s in " COLOR_YELLOW "%s" COLOR_NONE "\n",
           shell->nbr_jobs, shell->cur_user, truncated_dir);

    if (truncated_dir != shell->cur_dir) {
        free(truncated_dir);
    }

    printf(COLOR_RED "mysh>" COLOR_NONE " ");

}
