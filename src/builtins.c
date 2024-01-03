#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/shell_info.h"
#include "include/builtins.h"


// builtins.c sert à implémenter les commandes internes du shell

// Change le répertoire de travail actuel "cd"
int changer_repertoire(int argc, char **argv) {
    char temp[PATH_BUFSIZE];
    strncpy(temp, shell->cur_dir, PATH_BUFSIZE);  // enregistrer le répertoire courant

    // détermine les commandes
    char *target_dir = (argc == 1) ? shell->pw_dir :  // si pas d'argument, aller au répertoire personnel
                      (strcmp(argv[1], "-") == 0) ? shell->prev_dir :  // si ya -, aller au répertoire précédent
                      argv[1];  // sinon, aller au répertoire spécifié

    // change le répertoire de travail actuel
    if (chdir(target_dir) != 0) {
        perror("bash: cd: NONEXISTENT");
        return 1;
    }

    strncpy(shell->prev_dir, temp, PATH_BUFSIZE);  // mettre à jour le répertoire précédent
    mise_a_jour_repertoire_courant();  // mettre à jour le répertoire courant

    return 0;
}




// Affiche le répertoire de travail actuel "pwd" 
int afficher_repertoire(){ 
    char cwd[PATH_BUFSIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd: Erreur lors de l'obtention du répertoire");
        return 1;
    }
    return 0;
}

// Quitte le shell avec un statut donné
void quitter_shell(int statut) {
    exit(statut);
}


// Met à jour le répertoire de travail actuel dans les informations du shell 
void mise_a_jour_repertoire_courant() {
     if (getcwd(shell->cur_dir, PATH_BUFSIZE) == NULL) {
        perror("Erreur mise_a_jour_repertoire_courant");
    }
}


// fonction pour afficher le dernier statut "?"
int afficher_dernier_statut() {
   printf("%d\n", shell->dernier_statut);
return 0;
}

// Ajouter d'autres fonctions de commandes internes apres

int has_active_jobs(struct shell_info *shell) {
    struct job *current_job = shell->root;
    while (current_job != NULL) {
        if (current_job->etat == 1 /* Running */ || current_job->etat == 2 /* Stopped */) {
            return 1; // Active job found
        }
        current_job = current_job->next;
    }
    return 0; // No active jobs
}

int handle_exit_command(char **tokens, struct shell_info *shell) {
    int exit_status = (tokens[1] != NULL) ? atoi(tokens[1]) : shell->dernier_statut;

    if (has_active_jobs(shell)) {
        fprintf(stderr, "There are jobs still running or suspended.\n");
        return 1; // Keep shell running
    }

    exit(exit_status);
}



// Exit command implementation
int shell_exit(char **args, struct shell_info *shell) {
    int exit_status = (args[1] != NULL) ? atoi(args[1]) : shell->dernier_statut;

    if (has_active_jobs(shell)) {
        fprintf(stderr, "There are jobs still running or suspended.\n");
        return 1; // Do not exit, return control to the shell
    }

    exit(exit_status); // Terminate the shell with the given status
}