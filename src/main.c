#include "include/main.h"

void main_loop()
{
  char input[INPUT_SIZE];
  char current_dir[1024];

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
// int main() {
//     initialize_shell();
//     simulate_shell_behavior();

//     // Free allocated memory for shell
//     free(shell);

//     return 0;
// }



// int main() {
//     // Allocate memory for shell
//     shell = malloc(sizeof(struct shell_info));
//     if (!shell) {
//         fprintf(stderr, "Failed to allocate memory for shell\n");
//         return 1;
//     }

//     // Initialize shell structure for testing
//     strcpy(shell->cur_user, "test_user");
//     strcpy(shell->cur_dir, "/");
//     strcpy(shell->pw_dir, "/");
//     shell->dernier_statut = 0;

//     // Run the tests
//     test_changer_repertoire();
//     test_afficher_repertoire();
//     test_quitter_shell();
//     test_afficher_dernier_statut();

//     // Signal end of tests
//     test(true, NULL);

//     // Free the allocated memory for shell
//     free(shell);
//     return 0;
// }

// //test
// int main() {
//     // Allocate memory for the shell_info structure
//     shell = malloc(sizeof(struct shell_info));
//     if (!shell) {
//         fprintf(stderr, "Failed to allocate memory for shell_info\n");
//         return EXIT_FAILURE;
//     }

//     // Initialize the shell_info structure
//     strcpy(shell->cur_user, "test_user");
//     strcpy(shell->cur_dir, "/home/test_user");
//     strcpy(shell->pw_dir, "/home");
//     shell->nbr_jobs = 3; // Example number of jobs

//     // Test the prompt functionality
//     char *prompt_output = afficher_prompt();
//     if (prompt_output) {
//         printf("Prompt output: %s\n", prompt_output);
//         free(prompt_output);
//     } else {
//         printf("Prompt generation failed.\n");
//     }

//     // Clean up
//     free(shell);

//     return EXIT_SUCCESS;
// }