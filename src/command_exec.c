#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"

int exec_command(char **tokens) {
    if (strcmp(tokens[0], "pwd") == 0) {
        // pwd
        printf("pwd\n");
        return 0;
    } else if (strcmp(tokens[0], "cd") == 0) {
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
        execvp(tokens[0], tokens);
        perror("");
        exit(1);
    }
    return 0;
}

// int main(void) {

//     char command[] = "cd -";
//     char *tokens[10];
//     parse_command(command, tokens);
//     exec_command(tokens);
//     return 0;

// }

