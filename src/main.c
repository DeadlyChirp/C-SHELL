#include "include/main.h"

void main_loop()
{
  char input[INPUT_SIZE];
  char current_dir[1028];

  do
  {
    printf("Entrez un mot : ");
    fgets(input, sizeof(input), stdin);

    // TODO: remplacer par gestion + parsing etc
    printf("Vous avez saisi : %s \n", input);

    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            printf("repo courant : %s\n", current_dir);
        } else {
            perror("getcwd");
        }
    // Suppression du caractère de nouvelle ligne s'il est présent
    input[strcspn(input, "\n")] = '\0';
  } while (strcmp("exit", input));
  
}

int main(int argc, char **argv)
{

  // boucle principale
  main_loop();

  return EXIT_SUCCESS;
}
