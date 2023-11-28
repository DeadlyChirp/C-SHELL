#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_command(char *command, char **tokens) {
    
    char *token = strtok(command, " "); // découpe command en tokens séparés par des espaces
    int i = 0;
    while (token != NULL) { // tant qu'il y a des tokens
        tokens[i] = token; // on ajoute le token à un tableau de tokens
        token = strtok(NULL, " "); // on passe au token suivant
        i++; // on incrémente i
    }
    tokens[i] = NULL; // on termine le tableau de tokens par NULL
    return 0;
}


