#ifndef COMMAND_H
#define COMMAND_H

int cmdHandler(char **args);
void RunExternalCmd(char **args);

extern char commands;
extern volatile sig_atomic_t pid_track;
extern int exit_status;

#endif