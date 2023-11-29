#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <command_parser.h>
#include <command_exec.h>
#include <shell_info.h>
#include <prompt.h>


//taille max de l'input est de 30 charactère + 1 pour \0
#define INPUT_SIZE 31