#include "include/main.h"

void main_loop()
{
  char input[INPUT_SIZE];
  //char current_dir[1024];
  char *tokens[16];
  struct shell_info *shell=malloc(sizeof(struct shell_info));

  if (shell == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  do
  {
    // initialise le répertoire courant
    if (getcwd(shell->cur_dir, sizeof(shell->cur_dir)) != NULL){
      printf("repo courant : %s\n", shell->cur_dir);
    }
    else{
      perror("getcwd");
    }

    printf("Entrez commande : ");
    fgets(input, sizeof(input), stdin);

    
    parse_command(input, tokens);    

    exec_command(tokens);

    // Suppression du caractère de nouvelle ligne s'il est présent
    input[strcspn(input, "\n")] = '\0';
  } while (strcmp("exit", input));
}

int main()
{

  // boucle principale
  main_loop();

  return EXIT_SUCCESS;
}
