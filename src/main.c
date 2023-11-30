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

  do {
    input = afficher_prompt(shell);

    if (input == NULL) {
      //printf("\n");
      break; 
    }

    parse_command(input, tokens);    
    shell->dernier_statut = exec_command(tokens);

    free(input); 
  } while (strcmp("exit", input) != 0);

}

int main() {
    shell = malloc(sizeof(struct shell_info));
    if (shell == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    //initialiser le nom de l'utilisateur
     char *username = getlogin();
    if (!username) {
        struct passwd *pw = getpwuid(geteuid()); // getpwuid() retourne un pointeur sur une structure statique contenant les informations de l'utilisateur
        if (pw) {
            username = pw->pw_name; // pw_name est le nom d'utilisateur
        } else {
            fprintf(stderr, "Erreur lors de l'obtention du nom d'utilisateur\n");
            username = "unknown";
        }
    }
    strncpy(shell->cur_user, username, USERNAME_BUFSIZE);
    shell->cur_user[USERNAME_BUFSIZE - 1] = '\0';

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