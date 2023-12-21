#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"
#include "include/shell_info.h"
#include "sys/wait.h"
#include "sys/types.h"
#include "fcntl.h"


int exec_command(char **tokens) {
     int status; 
    if (strcmp(tokens[0], "pwd") == 0) {
        //pwd
        shell->dernier_statut = afficher_repertoire();
        //printf("pwd\n");
    } else if (strcmp(tokens[0], "cd") == 0) {
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
        shell->dernier_statut = afficher_dernier_statut();
        // return 0;
    } else  if (strcmp(tokens[0], "exit") == 0) {
        int exit_status = shell->dernier_statut; // exit avec le statut de la dernière commande 
        if (tokens[1] != NULL) {
            exit_status = atoi(tokens[1]); // Override si un argument est fourni
    } 

    if (shell->nbr_jobs > 0) {
        fprintf(stderr, "There are jobs still running or suspended.\n");
        shell->dernier_statut = 1; // met à jour le statut sans quitter
    } else {
        exit(exit_status); // Exit le shell avec le statut donné
    }
}else {
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


int exec_command_redirection(char **tokens){
    if (strcmp(tokens[1], "<") == 0){
        // <
        printf("redirection <\n");
    } else if (strcmp(tokens[1], ">") == 0){
        // >
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int fd = open(tokens[2], O_WRONLY | O_CREAT | O_EXCL, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            shell->dernier_statut = exec_command(tokens);
            exit(shell->dernier_statut);
        } else {
            waitpid(pid, NULL, 0);
        }        
    } else if (strcmp(tokens[1], ">|") == 0){
        // >|
        printf("redirection >|\n");
    } else if (strcmp(tokens[1], ">>") == 0){
        // >>
        printf("redirection >>\n");
    } else if (strcmp(tokens[1], "2>") == 0){
        // 2>
        printf("redirection 2>\n");
    } else if (strcmp(tokens[1], "2>>") == 0){
        // 2>>
        printf("redirection 2>>\n");
    } else if (strcmp(tokens[1], "2>|") == 0){
        // 2>|
        printf("redirection 2>|\n");
    } else if (strcmp(tokens[1], "|") == 0){
        // |
        printf("redirection |\n");
    } else if (strcmp(tokens[1], "<(") == 0){
        // <(
        printf("redirection <(\n");
    } else {
        printf("redirection inconnue\n");
    }
    return shell->dernier_statut;
}

