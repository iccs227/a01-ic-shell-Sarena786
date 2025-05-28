#ifndef COMMAND_H
#define COMMAND_H
#include <signal.h>

int cmdHandler(char **args);
void RunExternalCmd(char **args, const char *cmdline);

extern char commands;
extern volatile sig_atomic_t pid_track;

#endif