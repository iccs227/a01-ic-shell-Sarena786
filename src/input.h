#include "stdio.h"
#ifndef INPUT_H
#define INPUT_H

#define MAX_CMD_BUFFER 255
extern int check_redirect;
extern int is_bg;
extern char cmdline[256];

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);
void Run(FILE *fp);

#endif
