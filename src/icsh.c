/* ICCS227: Project 1: icsh
 * Name: Sareena Aulla
 * StudentID: 6481197
 */

#include <stdio.h>
#include "input.h"
#include "command.h"
#include "signal.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "command.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_BUFFER 255

pid_t shell_id;

int main(int argc, char *argv[]) {

    setpgid(0, 0);
    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    
    FILE *fp = NULL;

    if(argc > 1) {
        fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("Cannot Open the file");
            return -1;
        }
    }

    printf("Starting IC Shell\n");

    signalHandler();
    Run(fp);
    
    if(fp != NULL) {
        fclose(fp);
    }

    return 0;
}
