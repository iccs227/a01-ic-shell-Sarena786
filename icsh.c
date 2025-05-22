/* ICCS227: Project 1: icsh
 * Name: Sareena Aulla
 * StudentID: 6481197
 */

#include "stdio.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD_BUFFER 255

int main() {
    char buffer[MAX_CMD_BUFFER];
    shell();
    while (1) {
        if(getInput(buffer) == -1) {
            continue;
        }
        
    }
}

int shell(){
    printf("Starting IC Shell\n");
    printf("icsh $ ");
}

int getInput(char *buffer) {
    if(fgets(buffer, MAX_CMD_BUFFER, stdin) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        return 0;
    } else {
        return -1;
    }
}

void execute(char **args) {
    pid_t pid = fork();
    if(pid == -1) {
        printf("Error forking process\n");
        return; 
    } else if (pid == 0) {
        if(execvp(args[0], args) < 0) {
            printf("Error executing command\n");
        }
        exit(0);
    } else {
        wait(NULL); 
    }
}