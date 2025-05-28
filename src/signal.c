#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "signal.h"
#include "job.h"
#include "input.h"
#include <string.h>

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
            if (sip->si_pid == pid_track) {
                exit_status = WEXITSTATUS(status);
                pid_track = -1;
                printf("The child exited.\n");
            } else {
                for (int i = 0; i < current_job; i++) {
                    if (jobs[i].pid == sip->si_pid) {
                        strncpy(jobs[i].status, "Done", sizeof(jobs[i].status));
                        printf("[%d]+  Done\t\t%s\n", jobs[i].job_id, jobs[i].command);
                        jobs[i].pid = -1;
                        break;
                    }
                }
            }
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


