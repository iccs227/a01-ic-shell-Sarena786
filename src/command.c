#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "command.h"
#include "redirect.h"
#include "input.h"

extern volatile sig_atomic_t pid_track;
extern int exit_status;
extern int is_bg;
#define MAX_JOBS 256
job jobs[MAX_JOBS];

typedef struct {
    int job_id;
    char status[16];
    char command[256];
} job;

int cmdHandler(char **args) {

    if(args[0] == NULL) {
        return 0;
    }

    if(strcmp(args[0], "echo") == 0) {
        for(int i = 1; args[i] != NULL; i++) {
            if (strcmp(args[i], "$?") == 0) {
                printf("%d ", exit_status);
            } else {
                printf("%s ", args[i]);
            }      
        }
        printf("\n");
        exit_status = 0;
        return 1;
    }

    else if (strcmp(args[0], "exit") == 0) {
        int code = 0;
        if(args[1] != NULL) {
            code = atoi(args[1]) & 0xFF;
        }
        printf("Bye\n");
        exit(code);
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
        redirect(args);
        execvp (args[0], args); // only return when there is an error
        printf("bad command\n");
        exit(1);
    }

    if(pid && is_bg != 1) {
        pid_track = pid;
        waitpid(pid, NULL, 0);
        pid_track = 0;
    } else {
        job_id 
    }

} 