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