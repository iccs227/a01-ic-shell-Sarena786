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
#include <errno.h>
#include <signal.h>

#define MAX_CMD_BUFFER 255

int getInput(char *buffer, FILE *fp);
void ParseInput(char *buffer, char **args);
int cmdHandler(char **args);
void Run(FILE *fp);
void RunExternalCmd(char **args);
void signalHandler();
void ChildHandler(int sig, siginfo_t *sip, void *notused);
void SIGINTHandler(int sig);
void SIGTSTPHandler(int sig);

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
        execvp (args[0], args); // only return when there is an error
        printf("bad command\n");
        exit(1);
    }

    if(pid) {
        pid_track = pid;
        waitpid(pid, NULL, 0);
        pid_track = 0;
    }
} 

void ChildHandler(int sig, siginfo_t *sip, void *notused) {
    int status;
    fflush (stdout);

    status = 0; 

    if(waitpid (sip->si_pid, &status, WNOHANG) > 0) {
        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            printf ("The child exited.\n");
        } else if (WIFSIGNALED(status)) {
            exit_status = 128 + WTERMSIG(status);
            printf("The child terminated.\n");
        }
        else
         printf ("Uninteresting\n");
    }
}

void SIGINTHandler(int sig) {
    if(pid_track > 0) {
        kill(pid_track, SIGINT);
        printf("Child process %d killed.\n", pid_track);
    }
}

void SIGTSTPHandler(int sig) {
    if(pid_track > 0) {
        kill(pid_track, SIGTSTP);
        printf("Child process %d suspended.\n", pid_track);
    }
}

void signalHandler() {

    struct sigaction action;
    action.sa_sigaction = ChildHandler;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &action, NULL);

    struct sigaction sa_int;
    sa_int.sa_handler = SIGINTHandler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_tstp;
    sa_tstp.sa_handler = SIGTSTPHandler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_tstp, NULL);

}