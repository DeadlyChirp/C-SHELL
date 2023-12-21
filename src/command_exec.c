#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/command_parser.h"
#include "include/builtins.h"
#include "include/shell_info.h"
#include "sys/wait.h"

int exec_command(char **tokens)
{
    if (shell == NULL) {
        fprintf(stderr, "Error: shell is NULL\n");
        return 1;
    }

    shell->dernier_statut = afficher_dernier_statut();

    if (tokens == NULL) {
        fprintf(stderr, "Error: tokens is NULL\n");
        return shell->dernier_statut;
    }

    int status;
    if (strcmp(tokens[0], "pwd") == 0)
    {
        // pwd
        shell->dernier_statut = afficher_repertoire();
        // printf("pwd\n");
    }
    else if (strcmp(tokens[0], "cd") == 0)
    {
        if (tokens[1] == NULL)
        {
            // cd
            shell->dernier_statut = changer_repertoire(1, tokens);
        }
        else if (strcmp(tokens[1], "-") == 0)
        {
            // cd -,
            tokens[1] = shell->prev_dir; // Mettre le repertoire precedent dans le tableau de tokens
            shell->dernier_statut = changer_repertoire(2, tokens);
        }
        else
        {
            // cd <ref>
            shell->dernier_statut = changer_repertoire(2, tokens);
        }
    }
    else if (strcmp(tokens[0], "?") == 0)
    {
        // ?
        shell->dernier_statut = afficher_dernier_statut();
        // return 0;
    }
    else if (strcmp(tokens[0], "exit") == 0)
    {
        int exit_status = shell->dernier_statut; // exit avec le statut de la dernière commande
        if (tokens[1] != NULL)
        {
            exit_status = atoi(tokens[1]); // Override si un argument est fourni
        }

        if (shell->nbr_jobs > 0)
        {
            fprintf(stderr, "There are jobs still running or suspended.\n");
            shell->dernier_statut = 1; // met à jour le statut sans quitter
        }
        else
        {
            exit(exit_status); // Exit le shell avec le statut donné
        }
    }
    else
    {
        // if ( & est dans la commande ) remplacer 0 par 1
        init_job(tokens, shell, 0);
    }
    return shell->dernier_statut; // Retour du statut de sortie de la commande
}
