#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/shell_info.h"
#include "include/builtins.h"


// builtins.c sert à implémenter les commandes internes du shell

// Change le répertoire de travail actuel "cd"
int changer_repertoire(int argc, char **argv) {
    char temp[PATH_BUFSIZE];
    strncpy(temp, shell->cur_dir, PATH_BUFSIZE);  // enregistrer le répertoire courant

    // Determine target directory
    char *target_dir = (argc == 1) ? shell->pw_dir :  // si pas d'argument, aller au répertoire personnel
                      (strcmp(argv[1], "-") == 0) ? shell->prev_dir :  // si ya -, aller au répertoire précédent
                      argv[1];  // sinon, aller au répertoire spécifié

    // Attempt to change directory
    if (chdir(target_dir) != 0) {
        perror("cd: Erreur lors du changement de répertoire");
        return 1;
    }

    strncpy(shell->prev_dir, temp, PATH_BUFSIZE);  // mettre à jour le répertoire précédent
    mise_a_jour_repertoire_courant();  // mettre à jour le répertoire courant

    return 0;
}




// Affiche le répertoire de travail actuel "pwd" 
int afficher_repertoire(){ 
    char cwd[PATH_BUFSIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
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
