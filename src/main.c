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
    shell = malloc(sizeof(PATH_BUFSIZE));
    if (getcwd(shell->cur_dir, PATH_BUFSIZE) != NULL){
      printf("repo courant : %s\n", shell->cur_dir);
    }
    else{
       perror("getcwd() error");
    }

    printf("Entrez commande : ");
    fgets(input, INPUT_SIZE, stdin);

    
    parse_command(input, tokens);    

    shell->dernier_statut = exec_command(tokens);

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
