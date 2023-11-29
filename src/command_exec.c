#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"

int exec_command(char **tokens, struct shell_info shell) {
    if (strcmp(tokens[0], "pwd") == 0) {
        // pwd
        printf("pwd\n");
        return 0;
    } else if (strcmp(tokens[0], "cd") == 0) {
        printf("acces \n");
        if (tokens[1] == NULL) {
            // cd
            printf("cd\n");
            return 0;
        } else if (strcmp(tokens[1], "-") == 0) {
            // cd -
            printf("cd -\n");
            return 0;
        } else {
            // cd <ref>
            printf("cd <ref>\n");
            return 0;
        }
    } else if (strcmp(tokens[0], "?") == 0) {
        // ?
        printf("?\n");
        return 0;
    } else if (strcmp(tokens[0], "exit") == 0) {
        // exit
        printf("exit\n");
        return 0;
    } else {
        // Check if the file exists and is executable
        if (access(tokens[0], X_OK) == 0) {
            printf("execvp can be executed\n");
            execvp(tokens[0], tokens);
        } else {
            printf("execvp cannot be executed\n");
        }
    }
    return 0;
}

