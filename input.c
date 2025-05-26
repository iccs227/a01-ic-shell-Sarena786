#include <stdio.h>
#include <string.h>
#include "input.h"
#include "command.h"

int getInput(char *buffer, FILE *fp) {
    if(fp != NULL) {
        fgets(buffer, MAX_CMD_BUFFER, fp);
        buffer[strcspn(buffer, "\n")] = 0;
        return 0;
    } else if (fgets(buffer, MAX_CMD_BUFFER, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        return 0;
    }
    return -1;
}

void ParseInput(char *buffer, char **args) {
    char *token = strtok(buffer, " ");
    int i = 0;
    while(token != NULL) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
}

void Run(FILE *fp) {
    char buffer[MAX_CMD_BUFFER];
    char lastCommand[MAX_CMD_BUFFER] = {0};
    char *args[64];

    while (1) {
        if(fp == NULL) {
            printf("icsh $ ");
        }

        if(getInput(buffer, fp) == -1) {
            if(fp != NULL) {
                break;
            }
            clearerr(stdin);
            continue;
        }

        if(buffer[0] == '\0' || buffer[0] == '#') {
            continue;
        }

        if(strcmp(buffer, "!!") == 0) {
            printf("%s\n", lastCommand);
            strcpy(buffer, lastCommand);
        } else {
            strcpy(lastCommand, buffer);
        }

        ParseInput(buffer, args);
        if(args[0] == NULL) {
            continue;
        }

        if(!cmdHandler(args)) {
            RunExternalCmd(args);
        }
    }
}