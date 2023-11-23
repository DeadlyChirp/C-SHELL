#include "main.h"

int main(int argc, char **argv)
{

  // boucle principale
  main_loop();

  return EXIT_SUCCESS;
}

void main_loop()
{
  const char input[INPUT_SIZE];

  do
  {
    printf("Entrez un mot : ");
    fgets(input, sizeof(INPUT_SIZE), stdin);

    // TODO: remplacer par gestion + parsing etc
    printf("Vous avez saisi : %s", input);

    // // Suppression du caractère de nouvelle ligne s'il est présent
    // mot[strcspn(mot, "\n")] = '\0';

    printf("Vous avez saisi : %s\n", input);
  } while (strcmp ("exit", input));
  
}