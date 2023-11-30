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
      printf("\n");
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
        struct passwd *pw = getpwuid(geteuid());
        if (pw) {
            username = pw->pw_name;
        } else {
            fprintf(stderr, "Erreur lors de l'obtention du nom d'utilisateur\n");
            username = "unknown";
        }
    }
    strncpy(shell->cur_user, username, USERNAME_BUFSIZE);
    shell->cur_user[USERNAME_BUFSIZE - 1] = '\0';

    main_loop();

    free(shell);
    return EXIT_SUCCESS;
}