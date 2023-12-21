#include "include/main.h"
#include "include/shell_info.h" 


#include <unistd.h>  
#include <pwd.h> 

struct shell_info *shell = NULL; 
void main_loop() {
    char *input;
    char *tokens[16];
    char *redirect[] ={"<", ">", ">|", ">>", "2>", "2>>", "2>|", "|", "<("} ;
    size_t redirect_size = sizeof(redirect)/sizeof(redirect[0]);

    if (getcwd(shell->cur_dir, PATH_BUFSIZE) != NULL) {
        printf("repo courant : %s\n", shell->cur_dir);
    } else {
        perror("getcwd() error");
    }

    while (1) {
        input = afficher_prompt(shell);

        // si l'entrée est NULL, cela signifie que l'utilisateur a appuyé sur Ctrl + D
        if (input == NULL) {
            exit(shell->dernier_statut);  // Exit le shell avec le statut donné (dernier statut enregistré)
        }

        // si l'entrée est vide, continuez
        if (strcmp(input, "") == 0) {
            free(input);
            continue;
        }

        
        parse_command(input, tokens); 


        int redirect_or_not = 0; //0 si pas de redirection, 1 si redirection

        for (unsigned i = 0; tokens[i] != NULL; i++) {
            for (unsigned j = 0; j< redirect_size; j++) {
                if (strcmp(tokens[i], redirect[j]) == 0 && tokens[i+1]){
                    char *redirect_symbole = tokens[i];
                    char *redirect_file = tokens[i+1];
                    redirect_or_not = 1;
                shell-> dernier_statut = exec_command_redirection(tokens, redirect_symbole, redirect_file);
                }
            }
        }   
        if (redirect_or_not == 0){
            shell->dernier_statut = exec_command(tokens); 
        }
        
        free(input);
        
    }
}


int main() {
    shell = malloc(sizeof(struct shell_info));
    if (shell == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    memset(shell, 0, sizeof(struct shell_info)); //Initialisation de la mémoire à 0
  
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
