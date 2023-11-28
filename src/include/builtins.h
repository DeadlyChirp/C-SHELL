#ifndef BUILTIN_H
#define BUILTIN_H

#include "shell_info.h"

// Déclaration de la fonction pour changer le répertoire de travail
int changer_repertoire(int argc, char **argv);

// Déclaration de la fonction pour afficher le répertoire de travail actuel
int afficher_repertoire(int argc, char **argv);

// Déclaration de la fonction pour quitter le shell
int quitter_shell(int argc, char **argv);

// Déclaration de la fonction pour mettre à jour le répertoire de travail actuel
void mise_a_jour_repertoire_courant();

// Déclaration de la fonction pour afficher le dernier statut
int afficher_dernier_statut(int argc, char **argv);

// Ajouter d'autres déclarations de fonctions de commandes intégrées si nécessaire...

#endif // BUILTIN_H
