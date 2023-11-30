#include "include/main.h"
#include "include/shell_info.h" // Make sure this is included if it's not already in main.h

struct shell_info *shell = NULL; // Declare the global shell pointer

void main_loop() {
  char *input;
  char *tokens[16];

  // Do not re-declare shell here, use the global shell pointer

  if (getcwd(shell->cur_dir, PATH_BUFSIZE) != NULL) {
    printf("repo courant : %s\n", shell->cur_dir);
  } else {
    perror("getcwd() error");
  }

  do {
    input = afficher_prompt(shell);

    if (input == NULL) {
      printf("\n");
      break; // Exit the loop if input is NULL (EOF or Ctrl-D)
    }

    parse_command(input, tokens);    
    shell->dernier_statut = exec_command(tokens);

    free(input); // Free the memory allocated by readline in afficher_prompt
  } while (strcmp("exit", input) != 0);

  // Do not free shell here, it will be freed in main
}

int main() {
  shell = malloc(sizeof(struct shell_info)); // Allocate memory for the shell
  if (shell == NULL) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  // Initialize the shell fields here

  main_loop(); // Call the main loop

  free(shell); // Free the shell structure at the end
  return EXIT_SUCCESS;
}
