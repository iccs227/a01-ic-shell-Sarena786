#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "command.h"
#include "redirect.h"
#include "input.h"
#include "job.h"
#include "signal.h"
#include <ctype.h>
#include <sys/types.h>
#include <termios.h>

extern volatile sig_atomic_t pid_fg;
extern int exit_status;
extern pid_t shell_id;

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

    else if (strcmp(args[0], "jobs") == 0) {
        printJobs();
        return 1;
    }

    else if (strcmp(args[0], "fg") == 0) {
        if(args[1] != NULL && args[1][0] == '%' && isdigit(args[1][1])) {
            int id = atoi(&args[1][1]); // convert string to number
            to_fg(id);
            return 1;
        }
    }
    else if (strcmp(args[0], "bg") == 0) {
         if(args[1] && args[1][0] == '%') {
            int job_id = atoi(args[1] + 1);
            cont_bg(job_id);
            return 1;
         }
    }

    else if (strcmp(args[0], "exit") == 0) {
        int code = 0;
        if(args[1] != NULL) {
            code = atoi(args[1]) & 0xFF;
        }
        printf("Bye\n");
        exit(code); // exit immediately
    }
    return 0;
}

void RunExternalCmd(char **args, const char *cmdline) {
    
    pid_t pid;
    
    if ((pid=fork()) < 0)
    {
        perror ("Fork failed");
        exit(errno);
    }

    if(!pid) { // child process
        setpgid(0, 0);

        if (!is_bg) {
            tcsetpgrp(STDIN_FILENO, getpid()); // terminal controls this process group
        }

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);

        redirect(args);
        execvp (args[0], args); // only return when there is an error

        printf("bad command\n");
        exit(1);
    }

    if(pid) { // parent process

        setpgid(pid, pid); // set child's process group
        // printf("Child PID: %d, PGID: %d\n", pid, getpgid(pid)); 

        if(!is_bg) {

            pid_fg = pid; // track current foreground pid for signal forwarding
            
            tcsetpgrp(STDIN_FILENO, pid); // give terminal control to foreground job

            int status;
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, shell_id);

            pid_fg = 0;

            exit_handler(status, pid, -1);
        } else {
            is_bg = 0;
            keepJob(pid, cmdline);
        }
    }
} 
