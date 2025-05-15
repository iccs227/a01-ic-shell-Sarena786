/* ICCS227: Project 1: icsh
 * Name: Sareena Aulla
 * StudentID: 6481197
 */

#include "stdio.h"
#include <sys/types.h>

#define MAX_CMD_BUFFER 255

int main() {
    char buffer[MAX_CMD_BUFFER];
    while (1) {
        printf("Starting IC Shell\n");
        printf("icsh $ ");
        fgets(buffer, 255, stdin);
        printf("%s\n", buffer);
    }
}

void execute(char **args) {
    pid_t pid = fork();
    if(pid == -1) {
        printf("Error forking process\n");
        return; 
    } else if (pid == 0) {
        if()
    }

}