#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"
#include "include/shell_info.h"

struct shell_info *shell;

int exec_command(char **tokens, struct shell_info shell) {
    if (strcmp(tokens[0], "pwd") == 0) {
        //pwd
        afficher_repertoire();
        printf("pwd\n");
        return 0;
    } else if (strcmp(tokens[0], "cd") == 0) {
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
            changer_repertoire(2,tokens[1]);
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
        quitter_shell(2,tokens[1]);
        printf("exit\n");
        return 0;
    } else {
        execvp(tokens[0], tokens);
        perror("");
        exit(1);
    }
    return 0;
}

