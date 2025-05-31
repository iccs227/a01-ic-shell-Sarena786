#include <stdio.h>
#include <string.h>
#include "input.h"
#include "command.h"
#include "redirect.h"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int check_redirect = 0;
int is_bg = 0;
char cmdline[256] = {0};

int getInput(char *buffer, FILE *fp) {
    if(fp != NULL) {
        fgets(buffer, MAX_CMD_BUFFER, fp); // use fgets to read a line from a file
        buffer[strcspn(buffer, "\n")] = 0; // clean a line
        return 0;   
    } else if (fgets(buffer, MAX_CMD_BUFFER, stdin) != NULL) { // reading from input
        buffer[strcspn(buffer, "\n")] = 0; // clean a line
        return 0;
    }
    return -1;
}

void ParseInput(char *buffer, char **args) {
    char *token = strtok(buffer, " "); // tokenize the input string
    int i = 0;

    while(token != NULL) {
        args[i] = token; // save each token into args[]

        token = strtok(NULL, " "); // continue parsing 

        if(strcmp(args[i],">") == 0 || strcmp(args[i],"<") == 0) { // to detect redirection
            check_redirect = 1;
        }
        if(strcmp(args[i], "&") == 0) { // to detect background execution
            is_bg = 1;
            args[i] = NULL;
            break;
        }
        i++;
    }
    args[i] = NULL; 
}

void Run(FILE *fp) {
    char buffer[MAX_CMD_BUFFER]; // store current line
    char lastCommand[MAX_CMD_BUFFER] = {0}; // store prev. command
    char *args[64]; // tokens

    while (1) {
        if(fp == NULL) {
            printf("icsh $ ");
        }

        if(getInput(buffer, fp) == -1) {
            if(fp != NULL) {
                break;
            }
            clearerr(stdin); // clear the error and prompt again
            continue;
        }

        if(buffer[0] == '\0' || buffer[0] == '#') {
            continue;
        }

        if(strcmp(buffer, "!!") == 0) {
            printf("%s\n", lastCommand);
            strcpy(buffer, lastCommand);
        } else {
            strcpy(lastCommand, buffer); // update last command
        }

        strcpy(cmdline, buffer); // save a copy of full command

        ParseInput(buffer, args); // break into separate tokens
        if(args[0] == NULL) {
            continue;
        }
        if(check_redirect == 0 && cmdHandler(args)) { // alrd executed in cmdHandler
        } else {
            RunExternalCmd(args, cmdline); 
        }
        check_redirect = 0;
    }
}