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

#define MAX_CMD_BUFFER 255

volatile sig_atomic_t pid_track = 0;
int exit_status = 0;

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
    
    signalHandler();
    Run(fp);
    
    if(fp != NULL) {
        fclose(fp);
    }

    return 0;
}
