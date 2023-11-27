#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <signal.h>
#include "shell_info.h"
#include "job.h"
#include "process.h"
#include "signal_handling.h"

extern struct shell_info *shell;

// Fonction pour gérer les signaux
void signal_handler(int sig_num) {
    switch(sig_num) {
       case SIGINT:
            printf("\nGroupe 59 jsh : Interruption par SIGINT (Ctrl+C)\n");
            break;
        case SIGTSTP:
            printf("\nroupe 59 jsh : Suspendu par SIGTSTP (Ctrl+Z)\n");
            // La logique de gestion des jobs en arrière-plan sera ajoutée plus tard
            break;
        case SIGQUIT:
            printf("\nGroupe 59jsh : Quitter par SIGQUIT (Ctrl+\\)\n");
            break;
        default:
            printf("\nGroupe 59jsh : Signal inattendu capturé %d\n", sig_num);
    }
    fflush(stdout); // Assurer que le flux de sortie est vidé
    mysh_print_promt();
}

// Function to Setup a Single Signal Handler
void setup_single_signal_handler(int signal, void (*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, NULL);
}

// fonction pour mettre en place les gestionnaires de signaux
void setup_signal_handlers() {
    setup_single_signal_handler(SIGINT, signal_handler);
    setup_single_signal_handler(SIGTSTP, signal_handler);
    setup_single_signal_handler(SIGQUIT, signal_handler);

    // ignorer les signaux SIGTTIN et SIGTTOU
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

}

//Oublie pas d'appeler setup_signal_handlers() dans init()
