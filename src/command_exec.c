#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"
#include "include/shell_info.h"
#include "sys/wait.h"


int exec_command(char **tokens) {
    if (strcmp(tokens[0], "pwd") == 0) {
        //pwd
        afficher_repertoire();
        //printf("pwd\n");
        return 0;
    } else if (strcmp(tokens[0], "cd") == 0) {
        printf("acces \n");
        if (tokens[1] == NULL) {
            // cd
            changer_repertoire(1,NULL); 
            //printf("cd\n");
            return 0;
        } else if (strcmp(tokens[1], "-") == 0) {
            // cd -
            printf("cd -\n");
            return 0;
        } else {
            // cd <ref>
            changer_repertoire(2,tokens);
            //printf("cd <ref>\n");
            return 0;
        }
    } else if (strcmp(tokens[0], "?") == 0) {
        // ?
        afficher_dernier_statut();
        printf("?\n");
        return 0;
    } else if (strcmp(tokens[0], "exit") == 0) {
        // exit
        quitter_shell(2,tokens);
        printf("exit\n");
        return 0;
    } else {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // processus enfant 
            execvp(tokens[0], tokens);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // Processus parent
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}

