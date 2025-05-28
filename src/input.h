#include "stdio.h"
#ifndef INPUT_H
#define INPUT_H

#define MAX_CMD_BUFFER 255
int check_redirect = 0;
int is_bg = 0;

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);
void Run(FILE *fp);

#endif
