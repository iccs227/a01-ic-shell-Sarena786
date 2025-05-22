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
    printf("Starting IC Shell\n");
    while (1) {
        printf("icsh $ ");
        fgets(buffer, 255, stdin);
        printf("%s\n", buffer);
    }
}

int getInput(char **str) {
    size_t len = 0;

    if(fgets(str, &len, stdin) != -1) {
        return str;
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