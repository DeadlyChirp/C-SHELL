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



// // fonction pour tronquer le chemin du répertoire si trop long
char *tronquer_chemin_repertoire(const char *dir, int max_length) {
    int dir_len = strlen(dir);
    if (dir_len <= max_length) {
        return strdup(dir);  // Si la longueur est inférieure ou égale à la longueur maximale, retourne le chemin complet.
    }

    // Calculer l'espace pour '...' et la fin du chemin
    const char *truncation_prefix = "...";
    int prefix_len = strlen(truncation_prefix);
    int num_chars_to_copy = max_length - prefix_len;  // Nombre de caractères à copier après '...'

    char *truncated = malloc(max_length + 1);  // +1 pour le caractère nul de fin
    if (!truncated) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // Copier '...' suivi de la fin du chemin
    strcpy(truncated, truncation_prefix);
    strncpy(truncated + prefix_len, dir + dir_len - num_chars_to_copy, num_chars_to_copy);
    truncated[max_length] = '\0';  // la chaîne est terminée par un caractère nul

    return truncated;
}


// fonction pour mettre à jour les informations sur le répertoire de travail actuel
void update_cwd_info(struct shell_info *shell) {
    char temp[PATH_MAX];
    if (getcwd(temp, sizeof(temp)) != NULL) {
        strncpy(shell->cur_dir, temp, PATH_BUFSIZE - 1);
        shell->cur_dir[PATH_BUFSIZE - 1] = '\0';  // on garanti que la chaîne est terminée par un caractère nul
    } else {
        perror("getcwd failed"); // perror() affiche un message d'erreur
    }
}

// Fonction pour afficher le prompt du shell

// Fonction pour afficher le prompt du shell
char *afficher_prompt(struct shell_info *shell) {
    update_cwd_info(shell);


    // Tronquer le chemin si nécessaire
    char *display_dir = tronquer_chemin_repertoire(shell->cur_dir, 25); // prompt taille limite : 30 caractères maximum

    // Obtenir le nombre de jobs actuellement gérés par le shell
    int job_count = shell->nbr_jobs; // Supposons que nbr_jobs est à jour

    // Modifier le format du prompt pour inclure le nombre de jobs
    char *prompt_format = "\033[32m[%d]\033[34m%s\033[00m$ ";
    int prompt_length = snprintf(NULL, 0, prompt_format, job_count, display_dir) + 1; // +1 pour le caractère nul de fin
    char *prompt = malloc(prompt_length);
    if (!prompt) {
        perror("Error allocating memory for prompt");
        exit(EXIT_FAILURE);
    }

    // Construction du prompt
    snprintf(prompt, prompt_length, prompt_format, job_count, display_dir);

    // Affichage du prompt sur sortie d'erreur standard
    fprintf(stderr, "%s", prompt);

    // Lecture de la ligne de commande sans afficher le prompt (car déjà fait)
    char *line = readline("");

    // Ajout de la ligne à l'historique si elle n'est pas vide
    if (line && *line) {
        add_history(line);
    }

    // Libération de la mémoire allouée
    if (display_dir != shell->cur_dir) {
        free(display_dir);
    }
    free(prompt);

    return line;
}








