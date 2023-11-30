#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/shell_info.h"
#include "include/builtins.h"

struct shell_info *shell;

// builtins.c sert à implémenter les commandes internes du shell

// Change le répertoire de travail actuel "cd"
int changer_repertoire(int argc, char **argv) {
    if (argc == 1) { //cd
        chdir(shell->pw_dir);
    } else {
        if (chdir(argv[1]) != 0) { //cd <ref> 
            fprintf(stderr, "cd: Impossible de changer pour le répertoire '%s'\n", argv[1]);
            return 1;
        }
    }
    mise_a_jour_repertoire_courant();
    return 0;
}


// Affiche le répertoire de travail actuel "pwd" 
int afficher_repertoire(){ 
    char cwd[PATH_BUFSIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Répertoire actuel: %s\n", cwd);
    } else {
        perror("pwd: Erreur lors de l'obtention du répertoire");
        return 1;
    }
    return 0;
}

// Quitte le shell "exit"
// Quitte le shell avec un statut donné
void quitter_shell(int statut_sortie) {
    exit(statut_sortie);
}


// Met à jour le répertoire de travail actuel dans les informations du shell 
void mise_a_jour_repertoire_courant() {
     if (getcwd(shell->cur_dir, PATH_BUFSIZE) == NULL) {
        perror("Erreur mise_a_jour_repertoire_courant");
    }
    // getcwd(shell->cur_dir, PATH_BUFSIZE);
}


// fonction pour afficher le dernier statut "?"
int afficher_dernier_statut() {
   printf("Valeur de retour de la dernière commande : %d\n", shell->dernier_statut);
return 0;
}

// Ajouter d'autres fonctions de commandes internes apres
