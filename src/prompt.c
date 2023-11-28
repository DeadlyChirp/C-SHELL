#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "include/prompt.h"
#include "include/shell_info.h"

struct shell_info *shell;

// fonction pour tronquer le chemin du répertoire si trop long
char *tronquer_chemin_repertoire(const char *dir, int max_length) {
    int dir_len = strlen(dir);
    if (dir_len <= max_length) {
        return strdup(dir);  // strdup() alloue de la mémoire pour la chaîne de caractères
    }

    char *truncated = malloc(max_length + 1);
    if (!truncated) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // Creer une chaine de caractères tronquée
    sprintf(truncated, "...%s", dir + dir_len - max_length + 3);
    return truncated;
}

// fonction pour mettre à jour les informations sur le répertoire de travail actuel
void update_cwd_info() {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
        shell->cur_dir[PATH_BUFSIZE - 1] = '\0';  // Garantir que la chaîne est terminée par un caractère nul
    } else {
        perror("getcwd failed"); // perror() affiche un message d'erreur
    }
}

// Fonction pour afficher le prompt du shell
char *afficher_prompt() {
    update_cwd_info();
    char *truncated_dir = tronquer_chemin_repertoire(shell->cur_dir, 20);

    char prompt[1024];
    sprintf(prompt, "\033[32m" "\001[%d]\002\033[33m" "\001 %s@%s\002\033[00m$ ", 
            shell->nbr_jobs, shell->cur_user, truncated_dir);


    char *line = readline(prompt);

    free(truncated_dir);  // Libérer la mémoire allouée par tronquer_chemin_repertoire()
    if (line && *line) {
        add_history(line);
    }

    return line;
}


