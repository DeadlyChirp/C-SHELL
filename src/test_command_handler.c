#include "jsh.h"
#include <assert.h>
#include <string.h>

void test_cd_command() {
    char *args[] = {"cd", "/tmp", NULL};
    assert(execute_command(args) == 1);
    // Vous pouvez ajouter des vérifications supplémentaires ici, par exemple, vérifier le répertoire courant après l'exécution

    char *args2[] = {"cd", NULL};
    assert(execute_command(args2) == 1);
    // Testez la réaction de la commande cd sans argument
}

void test_pwd_command() {
    char *args[] = {"pwd", NULL};
    assert(execute_command(args) == 1);
    // Ajoutez des vérifications pour le résultat de la commande pwd
}

void test_exit_command() {
    char *args[] = {"exit", NULL};
    assert(execute_command(args) == 1);
    // Tester la commande exit est délicat car elle termine le programme. Vous pourriez avoir besoin de l'adapter.
}

void test_external_command() {
    char *args[] = {"ls", NULL};
    assert(execute_command(args) == 1);
    // Testez avec une commande externe simple
}

int main() {
    test_cd_command();
    test_pwd_command();
    test_exit_command();
    test_external_command();

    printf("Tous les tests sont passés.\n");
    return 0;
}
