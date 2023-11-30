#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"
#include "include/shell_info.h"
#include "sys/wait.h"


int exec_command(char **tokens) {
     int status; 
    if (strcmp(tokens[0], "pwd") == 0) {
        //pwd
        shell->dernier_statut = afficher_repertoire();
        //printf("pwd\n");
    } else if (strcmp(tokens[0], "cd") == 0) {
        printf("acces \n");
        if (tokens[1] == NULL) {
            // cd 
            shell->dernier_statut = changer_repertoire(1, tokens); 
        } else if (strcmp(tokens[1], "-") == 0) {
            // cd -, 
            tokens[1] = shell->prev_dir; // Mettre le repertoire precedent dans le tableau de tokens
            shell->dernier_statut = changer_repertoire(2, tokens);
        } else {
            // cd <ref>
            shell->dernier_statut = changer_repertoire(2, tokens);
        }
    } else if (strcmp(tokens[0], "?") == 0) {
        // ?
        printf("?\n");
        shell->dernier_statut = afficher_dernier_statut();
        // return 0;
    } else if (strcmp(tokens[0], "exit") == 0) {
            if (tokens[1] != NULL) {
                shell->dernier_statut = atoi(tokens[1]);
            }else {
            shell->dernier_statut = 0;
        }
        quitter_shell(shell->dernier_statut);
    
    } else {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            shell->dernier_statut = EXIT_FAILURE;
        } else if (pid == 0) {
            // processus enfant 
            execvp(tokens[0], tokens);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // processus parent 
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                shell->dernier_statut = WEXITSTATUS(status); // Prendre le statut de sortie de l'enfant
            } else if (WIFSIGNALED(status)) {
                shell->dernier_statut = WTERMSIG(status); // commande tuée par un signal
            } else {
                shell->dernier_statut = EXIT_FAILURE; // Erreur inconnue
            }
        }
    }
    return shell->dernier_statut; // Retour du statut de sortie de la commande
}

