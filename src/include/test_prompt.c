#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include "prompt.h"
#include "shell_info.h"

struct shell_info *shell;

void initialize_shell() {
    shell = malloc(sizeof(struct shell_info));
    if (!shell) {
        fprintf(stderr, "Failed to allocate memory for shell\n");
        exit(EXIT_FAILURE);
    }

    // utilisateur courant
    getlogin_r(shell->cur_user, sizeof(shell->cur_user));

    // répertoire de travail de l'utilisateur
    if (getcwd(shell->cur_dir, sizeof(shell->cur_dir)) == NULL) {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

   //test pour le nombre de jobs
    shell->nbr_jobs = 5;
}

int main() {
    initialize_shell();
    mysh_print_prompt();

    // on libère la mémoire allouée pour le shell
    free(shell);

    return 0;
}
