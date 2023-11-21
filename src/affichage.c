//Gerer l'affichage, comme un prompt

#include <stdio.h>
#include <stdlib.h>
#include "jsh.h"
#define JSH_TOK_BUFSIZE 64
#define JSH_TOK_DELIM " \t\r\n\a"

void print_prompt() {
    printf("jsh$ "); // Prompt de base
}

char* read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; // getline alloue la mémoire nécessaire

    // getline lit une ligne entière, stocke l'adresse du tampon contenant cette ligne dans *line et la taille du tampon dans *bufsize
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            // EOF (fin de fichier) a été atteint, quittez le programme normalement.
            exit(EXIT_SUCCESS); 
        } else  {
            // Une erreur s'est produite.
            perror("jsh: getline\n");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}



char** split_line(char *line) {
    int bufsize = JSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "jsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, JSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += JSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "jsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, JSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
