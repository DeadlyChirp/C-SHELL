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
     int nb_symbole = 0; // nb de symboles redirections dans la commande




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

    // if ( & est dans la commande ) remplacer 0 par 1
    // init_job(tokens, shell, is_background_job(tokens), status);

int exec_command(char **tokens) {
     int status; 
     int is_bg = is_background_job(tokens);
     int symbole_indices[10]; //store les indices des différents symboles de redirection
        for (unsigned i = 0; tokens[i] != NULL; i++) {
                for (unsigned j = 0; j< redirect_size; j++) {
                    if (strcmp(tokens[i], redirect[j]) == 0){
                        symbole_indices[nb_symbole++] = i;
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
        if (nb_symbole==1){
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
            if (nb_symbole == 1){
                char **tokens2 = malloc(sizeof(char*) * 16);
                int i=0;
                while(tokens[i]!=NULL){
                    if (i == symbole_indices[nb_symbole-1]){
                        break;
                    }else{
                    tokens2[i] = tokens[i];
                    i++;
                    }
                }
                tokens2[i] = NULL;
                //int j = 0;
                // while(tokens2[j]!=NULL){
                //     printf("tokens2[%d] = %s\n", j, tokens2[j]);
                //     j++;
                // }
                // printf("tokens2[%d] = %s\n", j, tokens2[j]);
                execvp(tokens2[0], tokens2);
                free(tokens2);
                perror("execvp");
                exit(shell->dernier_statut);
            }else if (nb_symbole > 0) {

                int fd[2];
                //fd[0] = read end
                //fd[1] = write end
                int fd_in = 0;

                int i=0;
                while(i<nb_symbole){
                    

                    if (pipe(fd)==-1) {
                        perror("pipe");
                        exit(EXIT_FAILURE);

                    }

                  
                    pid_t pid = fork();
                    if(pid == -1){
                        perror("fork");
                        exit(EXIT_FAILURE);
                    }

                    if (pid == 0){
                        dup2(fd_in, 0);
                        if (i != nb_symbole-1){
                            dup2(fd[1], 1);
                        }
                        close(fd[0]);


                        char **toks2 = malloc(sizeof(char*) * 16);
                        int j = 0;
                        while(tokens[j]!=NULL){
                            if (j == symbole_indices[i]){
                                break;
                            }else{
                            toks2[j] = tokens[j];
                            printf("toks2[%d] = %s\n", j, toks2[j]);
                            j++;
                            }
                        }

                        execvp(toks2[0], toks2);
                        free(toks2);
                        perror("execvp");
                        exit(shell->dernier_statut);
                    }else{
                        wait(NULL);
                        close(fd[1]);
                        fd_in = fd[0];
                        i++;
                    }
                }
    // pid_t pid = fork();

    // if (pid == -1) {
    //     perror("fork");
    //     shell->dernier_statut = EXIT_FAILURE;
    // } else if (pid == 0) {
    //     for (int i = 0; i < nb_symbole; i++) {
    //         int fd;
    //         char *redirect_symbol = tokens[symbole_indices[i]];
    //         char *file_path = tokens[symbole_indices[i] + 1];

    //         int flags = 0;

    //         if (strcmp(redirect_symbol, "<") == 0) {
    //             fd = open(file_path, O_RDONLY);
    //         } else if (strcmp(redirect_symbol, ">") == 0) {
    //             flags = O_WRONLY | O_CREAT | O_TRUNC;
    //             fd = open(file_path, flags, 0666);
    //         } else if (strcmp(redirect_symbol, ">>") == 0) {
    //             flags = O_WRONLY | O_CREAT | O_APPEND;
    //             fd = open(file_path, flags, 0666);
    //         } else if (strcmp(redirect_symbol, "2>") == 0) {
    //             flags = O_WRONLY | O_CREAT | O_TRUNC;
    //             fd = open(file_path, flags, 0666);
    //         } else if (strcmp(redirect_symbol, "2>>") == 0) {
    //             flags = O_WRONLY | O_CREAT | O_APPEND;
    //             fd = open(file_path, flags, 0666);
    //         } else if (strcmp(redirect_symbol, ">|") == 0 || strcmp(redirect_symbol, "2>|") == 0) {
    //             flags = O_WRONLY | O_CREAT;
    //             fd = open(file_path, flags, 0666);
    //         } else {
    //             perror("Unsupported redirection");
    //             exit(EXIT_FAILURE);
    //         }

    //         if (fd == -1) {
    //             perror("open");
    //             exit(EXIT_FAILURE);
    //         }

    //         // Check for existing file and handle accordingly
    //         if ((flags & O_CREAT) && (flags & O_EXCL)) {
    //             // O_EXCL is set (file must not exist)
    //             fprintf(stderr, "File '%s' already exists.\n", file_path);
    //             exit(EXIT_FAILURE);
    //         }

    //         if (strcmp(redirect_symbol, "<") == 0) {
    //             dup2(fd, STDIN_FILENO);
    //         } else if (strcmp(redirect_symbol, ">") == 0 || strcmp(redirect_symbol, ">>") == 0) {
    //             dup2(fd, STDOUT_FILENO);
    //         } else if (strcmp(redirect_symbol, "2>") == 0 || strcmp(redirect_symbol, "2>>") == 0) {
    //             dup2(fd, STDERR_FILENO);
    //         }

    //         close(fd);
    //     }

    //     tokens[symbole_indices[0]] = NULL; // Terminate the command part
    //     execvp(tokens[0], tokens);
    //     perror("execvp");
    //     exit(EXIT_FAILURE);
    // } else {
    //     waitpid(pid, &status, 0);
    //     if (WIFEXITED(status)) {
    //         shell->dernier_statut = WEXITSTATUS(status);
    //     } else {
    //         shell->dernier_statut = EXIT_FAILURE;
    //     }
    // }


         } else{
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
        if (nb_symbole==1){
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


int handle_redirections(char **tokens) {
    int input_fd, output_fd, error_fd;

    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(pid == 0){

        // Loop through tokens to find and handle redirections
        for (int i = 0; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], "<") == 0) {
                // Input redirection
                input_fd = open(tokens[i + 1], O_RDONLY);
                if (input_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
                tokens[i] = NULL; // Remove redirection symbol
            } else if (strcmp(tokens[i], ">") == 0) {
                // Output redirection
                output_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (output_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
                tokens[i] = NULL; // Remove redirection symbol
            } else if (strcmp(tokens[i], "2>") == 0) {
                // Error redirection
                error_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (error_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(error_fd, STDERR_FILENO);
                close(error_fd);
                tokens[i] = NULL; // Remove redirection symbol
            }
        }

        // Execute the command with modified file descriptors
        shell -> dernier_statut =  exec_command(tokens);
        exit(shell->dernier_statut);
    }else{
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
