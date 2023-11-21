// implémentez une boucle infinie qui sera la boucle principale de votre shell. Cette boucle affichera l'invite de commande (prompt), lira l'entrée de l'utilisateur, et déterminera si l'entrée est une commande interne ou externe.

#include <stdio.h>
#include <stdlib.h>
#include "jsh.h"

int main() {
    char *line;
    char **args;
    int status;

    // Initialisation du shell (si nécessaire)
    // Par exemple, configuration des gestionnaires de signaux, initialisation de l'environnement, etc.

    do {
        print_prompt();    // Affiche le prompt
        line = read_line(); // Lit la ligne de commande entrée par l'utilisateur
        args = split_line(line); // Divise la ligne en arguments
        status = execute_command(args); // Exécute la commande

        free(line);
        free(args);
    } while (status);

    // Nettoyage et fermeture du shell (si nécessaire)
    
    return EXIT_SUCCESS;
}
