#include "include/main.h"
#include "include/shell_info.h" 


#include <unistd.h>  
#include <pwd.h> 

struct shell_info *shell = NULL; 
void main_loop() {
    char *input;
    char *tokens[16];

    if (getcwd(shell->cur_dir, PATH_BUFSIZE) != NULL) {
        printf("repo courant : %s\n", shell->cur_dir);
    } else {
        perror("getcwd() error");
    }

    while (1) {
        input = afficher_prompt(shell);

        // si l'entrée est NULL, cela signifie que l'utilisateur a appuyé sur Ctrl + D
        if (input == NULL) {
            exit(shell->dernier_statut);  // Exit with the last command's status when input is NULL
        }

        // si l'entrée est vide, continuez
        if (strcmp(input, "") == 0) {
            free(input);
            continue;
        }

        
        parse_command(input, tokens);
        shell->dernier_statut = exec_command(tokens);

        free(input);
        
    }
}


int main() {
    shell = malloc(sizeof(struct shell_info));
    if (shell == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

  
    //inittialiser le repertoire HOME pour cd tout seul 
    char *home_dir = getenv("HOME"); // getenv() retourne la valeur de la variable d'environnement
    if (home_dir) {
        strncpy(shell->pw_dir, home_dir, PATH_BUFSIZE);
        shell->pw_dir[PATH_BUFSIZE - 1] = '\0'; // assurer que la chaîne est terminée par un caractère nul
    } else {
        fprintf(stderr, "Erreur lors de l'obtention du répertoire personnel\n");
        strcpy(shell->pw_dir, "/"); // default est le répertoire racine si HOME n'est pas défini
    }

    main_loop();

    free(shell);
    return EXIT_SUCCESS;
}