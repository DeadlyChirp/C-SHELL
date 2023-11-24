#ifndef PROMPT_H
#define PROMPT_H
#define PATH_MAX 4096 // taille maximale du chemin
#define PATH_BUFSIZE 1024  // taille du buffer pour le chemin
#include "job.h"
//couleur du prompt
#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[1;37;41m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_GRAY "\033[1;30m"

// declaration de la fonction pour afficher le prompt du shell
void mysh_print_prompt();

// declaration de la fonction pour mettre à jour les informations sur le répertoire de travail actuel
void update_cwd_info();

// declaration de la fonction pour tronquer le chemin du répertoire si trop long
char *truncate_dir(char *dir, int max_length);

#endif // PROMPT_H
