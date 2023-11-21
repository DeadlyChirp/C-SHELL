//Traiter et executer les commandes entrees par l'utilisateur

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jsh.h"

/*
 * Fonction pour exécuter les commandes internes.
 * Retourne 1 si la commande est interne, 0 sinon.
 */
int execute_internal_command(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        // Gérer la commande 'cd'
        if (args[1] == NULL) {
            fprintf(stderr, "jsh: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("jsh");
            }
        }
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        // Gérer la commande 'exit'
        exit(EXIT_SUCCESS);
    } else if (strcmp(args[0], "pwd") == 0) {
        // Gérer la commande 'pwd'
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("jsh");
        }
        return 1;
    }
    // Si la commande n'est pas reconnue comme une commande interne
    return 0;
}

/*
 * Fonction pour lancer des commandes externes.
 */
int launch_external_command(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Processus enfant
        if (execvp(args[0], args) == -1) {
            perror("jsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Erreur lors du fork
        perror("jsh");
    } else {
        // Processus parent
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/*
 * Fonction principale pour exécuter une commande.
 */
int execute_command(char **args) {
    if (args[0] == NULL) {
        // Une commande vide a été entrée.
        return 1;
    }

    // Vérifie si la commande est interne et l'exécute si c'est le cas
    if (execute_internal_command(args)) {
        return 1;
    }

    // Exécute une commande externe si ce n'est pas une commande interne
    return launch_external_command(args);
}
