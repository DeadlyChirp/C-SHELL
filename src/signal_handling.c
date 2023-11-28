//MARCHE PAS DU TOUT


// #define _XOPEN_SOURCE 700
// #include <stdio.h>
// #include <signal.h>
// #include "shell_info.h"
// #include "job.h"
// #include "process.h"
// #include "signal_handling.h"

// struct shell_info *shell;

// // Fonction pour gérer les signaux
// void signal_handler(int sig_num) {
//     switch(sig_num) {
//        case SIGINT:
//             printf("\nGroupe 59 jsh : Interruption par SIGINT (Ctrl+C)\n");
//             break;
//         case SIGTSTP:
//             printf("\nroupe 59 jsh : Suspendu par SIGTSTP (Ctrl+Z)\n");
//             // La logique de gestion des jobs en arrière-plan sera ajoutée plus tard
//             break;
//         case SIGQUIT:
//             printf("\nGroupe 59jsh : Quitter par SIGQUIT (Ctrl+\\)\n");
//             break;
//         default:
//             printf("\nGroupe 59jsh : Signal inattendu capturé %d\n", sig_num);
//     }
//     fflush(stdout); // Assurer que le flux de sortie est vidé
//     mysh_print_promt();
// }

// // Function to Setup a Single Signal Handler
// void setup_single_signal_handler(int signal, void (*handler)(int)) {
//     struct sigaction sa;
//     memset(&sa, 0, sizeof(sa));
//     sa.sa_handler = handler;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     sigaction(signal, &sa, NULL);
// }

// // fonction pour mettre en place les gestionnaires de signaux
// void setup_signal_handlers() {
//     setup_single_signal_handler(SIGINT, signal_handler);
//     setup_single_signal_handler(SIGTSTP, signal_handler);
//     setup_single_signal_handler(SIGQUIT, signal_handler);

//     // ignorer les signaux SIGTTIN et SIGTTOU
//     signal(SIGTTIN, SIG_IGN);
//     signal(SIGTTOU, SIG_IGN);

// }

// //Oublie pas d'appeler setup_signal_handlers() dans init()
// //test 
// char last_printed[256]; // Buffer to store the last printed message

// // Mock function for `mysh_print_promt` used in signal handling
// void mysh_print_promt() {
//     strcat(last_printed, "Mock prompt: $ ");
// }

// // Function to test a specific signal
// // Function to test a specific signal
// void test_signal(int signal, const char *expected_output, const char *test_name) {
//     printf("Testing %s...\n", test_name);
//     strcpy(last_printed, ""); // Reset the buffer
//     signal_handler(signal);   // Call the signal handler
//     printf("Expected: \"%s\", Got: \"%s\"\n", expected_output, last_printed);
//     if (strcmp(expected_output, last_printed) == 0) {
//         printf("Test passed: %s\n\n", test_name);
//     } else {
//         printf("Test failed: %s\n\n", test_name);
//     }
// }

// int main() {
//     setup_signal_handlers();

//     // Test each signal
//     test_signal(SIGINT, "\nGroupe 59 jsh : Interruption par SIGINT (Ctrl+C)\nMock prompt: $ ", "SIGINT Handling");
//     test_signal(SIGTSTP, "\nGroupe 59 jsh : Suspendu par SIGTSTP (Ctrl+Z)\nMock prompt: $ ", "SIGTSTP Handling");
//     test_signal(SIGQUIT, "\nGroupe 59 jsh : Quitter par SIGQUIT (Ctrl+\\)\nMock prompt: $ ", "SIGQUIT Handling");
//     test_signal(SIGUSR1, "\nGroupe 59jsh : Signal inattendu capturé 10\nMock prompt: $ ", "Unexpected Signal Handling");

//     return 0;
// }
