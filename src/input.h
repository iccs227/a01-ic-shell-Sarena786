#include "stdio.h"
#ifndef INPUT_H
#define INPUT_H
#include <signal.h>

#define MAX_CMD_BUFFER 255
extern int check_redirect;
extern volatile sig_atomic_t is_bg;
extern char cmdline[256];

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);
void Run(FILE *fp);

#endif
