#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "signal.h"

extern volatile sig_atomic_t pid_track;
int exit_status = 0;

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

void ChildHandler(int sig, siginfo_t *sip, void *notused) {
    int status;
    fflush (stdout);

    status = 0; 

    if(waitpid (sip->si_pid, &status, WNOHANG | WUNTRACED) > 0) {
        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            printf ("The child exited.\n");
        } else if (WIFSIGNALED(status)) {
            exit_status = 128 + WTERMSIG(status);
            printf("The child was terminated.\n");
        } else if (WIFSTOPPED(status)) {
            exit_status = 128 + WSTOPSIG(status);
            printf("The child was suspended.\n");
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


