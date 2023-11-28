#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_command(char *command, char **tokens) {
    
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    tokens[i] = NULL;
    return 0;
}


