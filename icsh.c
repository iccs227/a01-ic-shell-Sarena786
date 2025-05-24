/* ICCS227: Project 1: icsh
 * Name: Sareena Aulla
 * StudentID: 6481197
 */

#include "stdio.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_BUFFER 255

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);
int cmdHandler(char **args);
void Run(FILE *fp);
void RunExternalCmd(char **args);

int main(int argc, char *argv[]) {
    FILE *fp = NULL;

    if(argc > 1) {
        fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("Cannot Open the file");
            return -1;
        }
    }

    printf("Starting IC Shell\n");

    Run(fp);
    
    if(fp != NULL) {
        fclose(fp);
    }

    return 0;
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
        
        cmdHandler(args);
    }
}

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

int cmdHandler(char **args) {

    if(args[0] == NULL) {
        return 0;
    }

    if(strcmp(args[0], "echo") == 0) {
        for(int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
            if(args[i + 1] == NULL) {
                printf(" ");
            }   
        }
        printf("\n");
    }

    else if (strcmp(args[0], "exit") == 0) {
        int code = 0;
        if(args[1] != NULL) {
            code = atoi(args[1]) & 0xFF;
        }
        printf("Bye\n");
        exit(code);
    } 
    
    else {
        printf("%s", "bad command\n");
    }
    
    return 0;
}

void RunExternalCmd(char **args) {
    int pid;

    if ((pid=fork()) < 0)
    {
        perror ("Fork failed");
        exit(errno);
    }

    if(!pid) {
        execvp (args[0], args);
    }

    if(pid) {
        waitpid(pid, NULL, 0);
    }
} 