#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_info.h"
#include "builtins.h"

extern struct shell_info *shell;

// builtins.c sert à implémenter les commandes internes du shell

// Change le répertoire de travail actuel
int changer_repertoire(int argc, char **argv) {
    if (argc == 1) {
        chdir(shell->pw_dir);
    } else {
        if (chdir(argv[1]) != 0) {
            fprintf(stderr, "Groupe 59: cd: Impossible de changer pour le répertoire '%s'\n", argv[1]);
            return 1;
        }
    }
    mise_a_jour_repertoire_courant();
    return 0;
}

// Affiche le répertoire de travail actuel
int afficher_repertoire(int argc, char **argv) {
    char cwd[PATH_BUFSIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Répertoire actuel: %s\n", cwd);
    } else {
        perror("Groupe 59 : pwd: Erreur lors de l'obtention du répertoire");
        return 1;
    }
    return 0;
}

// Quitte le shell
int quitter_shell(int argc, char **argv) {
    int statut_sortie = 0;
    if (argc > 1) {
        statut_sortie = atoi(argv[1]);
    }
    printf("Groupe 59: Fin du shell avec le statut %d\n", statut_sortie);
    exit(statut_sortie);
}

// Met à jour le répertoire de travail actuel dans les informations du shell
void mise_a_jour_repertoire_courant() {
    if (getcwd(shell->cur_dir, sizeof(shell->cur_dir)) == NULL) {
        perror("Groupe 59 : Erreur lors de la mise à jour du répertoire courant");
    }
}

// Ajouter d'autres fonctions de commandes internes apres
