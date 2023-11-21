//contient les declarations des fonctions et 
//les definitions des structures utilisees dans l'ensemble du projet

#ifndef JSH_H
#define JSH_H

// Pour les commandes internes
int jsh_cd(char **args);
int jsh_pwd(char **args);
int jsh_exit(char **args);

// Pour le traitement et l'exécution des commandes
int execute_command(char **args);
int execute_internal_command(char **args);
int launch_external_command(char **args);

#endif // JSH_H
