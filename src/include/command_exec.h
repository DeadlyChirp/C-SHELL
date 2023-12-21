#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H
#include <shell_info.h>

int exec_command(char **tokens);
int exec_command_redirection(char **tokens,char *redirect_symbole, char *redirect_file);

#endif