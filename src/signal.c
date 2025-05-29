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
#include "command.h"

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
        if (sip->si_pid == pid_track) { // foreground has ended
            pid_track = 0;
        } 
        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            printf("The child exited.\n");
        } else if (WIFSIGNALED(status)) {
            exit_status = 128 + WTERMSIG(status);
            printf("The child was terminated.\n");
        } else if (WIFSTOPPED(status)) {
            exit_status = 128 + WSTOPSIG(status);
            printf("The child was suspended.\n");
        }
        
        for (int i = 0; i < current_job; i++) {
            if (jobs[i].pid == sip->si_pid) {
                if (WIFEXITED(status)) {
                    jobs[i].pid = -1;
                    strcpy(jobs[i].status, "Done");
                    printf("[%d]+  Done\t\t%s\n", jobs[i].job_id, jobs[i].command);
                } else if (WIFSTOPPED(status) || WIFSIGNALED(status)) {
                    strcpy(jobs[i].status, "Stopped");
                    printf("[%d]+  Stopped\t\t%s\n", jobs[i].job_id, jobs[i].command);
                }
                break;
            }
        }
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


