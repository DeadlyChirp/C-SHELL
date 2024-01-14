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

    // pour afficher les tokens et enlever \0 à la fin de chaque folen
    // int j;
    // for (j = 0; tokens[j] != NULL; j++) {
    //   printf("Token %d: %s\n", j, tokens[j]);
    //   tokens[j][strcspn(tokens[j], "\n")] = '\0';
    // } 
    // return 0;
}


