#ifndef SIGNAL_HANDLING_H
#define SIGNAL_HANDLING_H

#include <signal.h>
#include "shell_info.h"  

// fonction de gestion des signaux
void sigint_handler(int sig_num); // SIGINT
void sigtstp_handler(int sig_num); // SIGTSTP
void sigquit_handler(int sig_num); // SIGQUIT


void setup_signal_handlers();


#endif // SIGNAL_HANDLING_H
