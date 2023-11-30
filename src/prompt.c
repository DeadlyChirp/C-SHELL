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
void update_cwd_info(struct shell_info *shell) {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
        shell->cur_dir[PATH_BUFSIZE - 1] = '\0';  // Garantir que la chaîne est terminée par un caractère nul
    } else {
        perror("getcwd failed"); // perror() affiche un message d'erreur
    }
}

// Fonction pour afficher le prompt du shell

char *afficher_prompt(struct shell_info *shell) {
    update_cwd_info(shell);

    char *home_dir = getenv("HOME");
    char *display_dir = shell->cur_dir;

    if (home_dir && strncmp(shell->cur_dir, home_dir, strlen(home_dir)) == 0) {
        display_dir = malloc(strlen(shell->cur_dir) - strlen(home_dir) + 2);
        if (!display_dir) {
            perror("Error allocating memory for display_dir");
            exit(EXIT_FAILURE);
        }
        strcpy(display_dir, "~");
        strcat(display_dir, shell->cur_dir + strlen(home_dir));
    }

    char *prompt_format = "\033[32m%s\033[32m@%s:\033[33m%s\033[00m$ ";
    int prompt_length = strlen(prompt_format) - 6 + strlen(shell->cur_user) + strlen(shell->cur_user) + strlen(display_dir) + 1;

    char *prompt = malloc(prompt_length);
    if (!prompt) {
        perror("Error allocating memory for prompt");
        exit(EXIT_FAILURE);
    }

    snprintf(prompt, prompt_length, prompt_format, shell->cur_user, shell->cur_user, display_dir);

    char *line = readline(prompt);

    if (line && *line) {
        add_history(line);
    }

    if (display_dir != shell->cur_dir) {
        free(display_dir);
    }
    free(prompt);

    return line;
}






