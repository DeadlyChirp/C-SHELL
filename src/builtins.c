#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_info.h"
#include "builtins.h"

struct shell_info *shell;

// builtins.c sert à implémenter les commandes internes du shell

// Change le répertoire de travail actuel "cd"
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

// Affiche le répertoire de travail actuel "pwd"
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

// Quitte le shell "exit"
int quitter_shell(int argc, char **argv) {
    int statut_sortie = 0;
    if (argc > 1) {
        statut_sortie = atoi(argv[1]);
    }
    printf("Groupe 59: exit :  Fin du shell avec le statut %d\n", statut_sortie);
    exit(statut_sortie);
}

// Met à jour le répertoire de travail actuel dans les informations du shell 
void mise_a_jour_repertoire_courant() {
    if (getcwd(shell->cur_dir, sizeof(shell->cur_dir)) == NULL) {
        perror("Groupe 59 : Erreur lors de la mise à jour du répertoire courant");
    }
}


// fonction pour afficher le dernier statut "?"
int afficher_dernier_statut(int argc, char **argv) {
    printf("Valeur de retour de la dernière commande : %d\n", shell->dernier_statut);
    return 0;
}
// Ajouter d'autres fonctions de commandes internes apres


//test 
#include <stdbool.h>
void test(bool condition, const char* test_name) {
    static int passed = 0, failed = 0;
    if (condition) {
        printf("Test passed: %s\n", test_name);
        passed++;
    } else {
        printf("Test failed: %s\n", test_name);
        failed++;
    }

    if (test_name == NULL) {  // End of tests
        printf("\nTotal tests: %d, Passed: %d, Failed: %d\n", passed + failed, passed, failed);
    }
}

void test_changer_repertoire() {
    char original_dir[PATH_BUFSIZE];
    getcwd(original_dir, sizeof(original_dir));

    // Test changing to a valid directory
    test(changer_repertoire(2, (char *[]){"cd", "/tmp"}) == 0, "Change to /tmp");

    // Test changing back to original directory
    test(changer_repertoire(2, (char *[]){"cd", original_dir}) == 0, "Change back to original directory");

    // Test changing to an invalid directory
    test(changer_repertoire(2, (char *[]){"cd", "/nonexistentdirectory"}) != 0, "Change to invalid directory");
}

void test_afficher_repertoire() {
    // Test if the function prints the current directory (manual verification required)
    test(afficher_repertoire(0, NULL) == 0, "Print current directory");
}

void test_quitter_shell() {
    // This function exits the program, so it's tricky to test it in a normal way.
    // You can comment out the actual `exit` call in quitter_shell for testing purposes.
    test(quitter_shell(2, (char *[]){"exit", "0"}) == 0, "Exit with status 0");
}

void test_afficher_dernier_statut() {
    // Assume the last status is set to 0
    shell->dernier_statut = 0;
    printf("Expected output for last status 0: 'Valeur de retour de la dernière commande : 0'\n");
    test(afficher_dernier_statut(0, NULL) == 0, "Display last status 0");

    // Change the last status and test again
    shell->dernier_statut = 1;
    printf("Expected output for last status 1: 'Valeur de retour de la dernière commande : 1'\n");
    test(afficher_dernier_statut(0, NULL) == 0, "Display last status 1");
}

int main() {
    // Allocate memory for shell
    shell = malloc(sizeof(struct shell_info));
    if (!shell) {
        fprintf(stderr, "Failed to allocate memory for shell\n");
        return 1;
    }

    // Initialize shell structure for testing
    strcpy(shell->cur_user, "test_user");
    strcpy(shell->cur_dir, "/");
    strcpy(shell->pw_dir, "/");
    shell->dernier_statut = 0;

    // Run the tests
    test_changer_repertoire();
    test_afficher_repertoire();
    test_quitter_shell();
    test_afficher_dernier_statut();

    // Signal end of tests
    test(true, NULL);

    // Free the allocated memory for shell
    free(shell);
    return 0;
}