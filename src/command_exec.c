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

char *redirect[] ={"<", ">", ">|", ">>", "2>", "2>>", "2>|", "|", "<("} ;
    size_t redirect_size = sizeof(redirect)/sizeof(redirect[0]);
     int has_symbole = 0;

char **split_tokens(char **tokens, int start, int end) {
    int length = end - start;
    char **new_tokens = malloc(sizeof(char*) * (length + 1)); // +1 for NULL terminator
    for (int i = 0; i < length; i++) {
        new_tokens[i] = tokens[start + i];
    }
    new_tokens[length] = NULL; // NULL terminate the new array
    return new_tokens;
}


int is_background_job(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "&") == 0) {
            tokens[i] = NULL; // Remove the '&' symbol from the tokens
            return 1;
        }
    }
    return 0;
}

    // if ( & est dans la commande ) remplacer 0 par 1
    // init_job(tokens, shell, is_background_job(tokens), status);

int exec_command(char **tokens) {
     int status; 
     int is_bg = is_background_job(tokens);
     int symbole_indices[10]; //store les indices des différents symboles de redirection
        for (unsigned i = 0; tokens[i] != NULL; i++) {
                for (unsigned j = 0; j< redirect_size; j++) {
                    if (strcmp(tokens[i], redirect[j]) == 0){
                        symbole_indices[has_symbole++] = i;
                        break;  
                    } 
                }
            }
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
        if (has_symbole==1){
            exit(exit_status);
        }else{
            if (tokens[1] != NULL){
                
               
                exit_status = atoi(tokens[1]); // Override si un argument est fourni
                }
            
        }

    if (shell->nbr_jobs > 0) {
        fprintf(stderr, "There are jobs still running or suspended.\n");
        shell->dernier_statut = 1; // met à jour le statut sans quitter
    } else {
        exit(exit_status); // Exit le shell avec le statut donné
    }
}else {
    char full_command[1024] = {0}; // Assuming 1024 is a sufficient length
    for (int i = 0; tokens[i] != NULL; i++) {
        strcat(full_command, tokens[i]);
        if (tokens[i + 1] != NULL) {
            strcat(full_command, " ");
        }
    }
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            shell->dernier_statut = EXIT_FAILURE;
        } else if (pid == 0) {
            if (has_symbole == 1){
                char **tokens2 = malloc(sizeof(char*) * 16);
                int i=0;
                while(tokens[i]!=NULL){
                    if (i == symbole_indices[has_symbole-1]){
                        break;
                    }else{
                    tokens2[i] = tokens[i];
                    i++;
                    }
                }
                tokens2[i] = NULL;
                execvp(tokens2[0], tokens2);
                free(tokens2);
                perror("execvp");
                exit(shell->dernier_statut);
            }else if (has_symbole > 1){
                for (int i = 0; i<has_symbole ; i++){
                    char **toks1 = split_tokens(tokens, 0, symbole_indices[i]);
                    char **toks2 = split_tokens(tokens, symbole_indices[i] + 1, symbole_indices[i+1] );
                    exec_command_redirection(toks1, tokens[symbole_indices[i]], toks2[0]);
                    free(toks1);
                    free(toks2);
                }
            }else{
                execvp(tokens[0], tokens);
                perror("execvp");
                exit(shell->dernier_statut);
            }
        } else{
            if (!is_bg) {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                shell->dernier_statut = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                shell->dernier_statut = WTERMSIG(status);
            } else {
                shell->dernier_statut = EXIT_FAILURE;
            }
        } else {
            // Background job handling
            printf("[%d] %d Running %s\n", shell->nbr_jobs++, pid, full_command);
            // Implement job tracking for background jobs here if needed
        }
    }
}

    return shell->dernier_statut;
}


int exec_command_redirection(char **tokens, char *redirect_symbole, char *redirect_file){
    if (strcmp(tokens[0], "exit") == 0) {
        int exit_status = shell->dernier_statut; // exit avec le statut de la dernière commande 
        if (has_symbole==1){
            exit(exit_status);
        }else{
            if (tokens[1] != NULL){
                
               
                exit_status = atoi(tokens[1]); // Override si un argument est fourni
                }
            
        }

    if (shell->nbr_jobs > 0) {
        fprintf(stderr, "There are jobs still running or suspended.\n");
        shell->dernier_statut = 1; // met à jour le statut sans quitter
    } else {
        exit(exit_status); // Exit le shell avec le statut donné
        }
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (strcmp(redirect_symbole, "<") == 0){
            // <    
            int fd = open(redirect_file, O_RDONLY);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }else if (strcmp(redirect_symbole, ">") == 0){
            // >
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_EXCL, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (strcmp(redirect_symbole, ">|") == 0){
            // >|
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (strcmp(redirect_symbole, ">>") == 0){
            // >>
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (strcmp(redirect_symbole, "2>") == 0){
            // 2>
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_EXCL, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else if (strcmp(redirect_symbole, "2>>") == 0){
            // 2>>
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else if (strcmp(redirect_symbole, "2>|") == 0){
            // 2>|
            int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else{
            // |
            printf("redirection |\n");
        }
        shell->dernier_statut = exec_command(tokens);
        exit(shell->dernier_statut);
    }else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            shell->dernier_statut = WEXITSTATUS(status); // Prendre le statut de sortie de l'enfant
        } else if (WIFSIGNALED(status)) {
            shell->dernier_statut = WTERMSIG(status); // commande tuée par un signal
        } else {
            shell->dernier_statut = EXIT_FAILURE; // Erreur inconnue
        }
    }
        return shell->dernier_statut;
}

