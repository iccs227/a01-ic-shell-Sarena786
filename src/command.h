#ifndef COMMAND_H
#define COMMAND_H
#include <signal.h>

int cmdHandler(char **args);
void RunExternalCmd(char **args, const char *cmdline);

extern volatile sig_atomic_t pid_fg;
extern int exit_status;
extern pid_t shell_id;

#endif