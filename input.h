#include "stdio.h"
#ifndef INPUT_H
#define INPUT_H

#define MAX_CMD_BUFFER 255

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);

#endif
