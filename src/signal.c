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
#include <sys/types.h>

// extern volatile sig_atomic_t pid_track;
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
    pid_t pid;
    fflush (stdout);


    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        printf("Debug: Caught child signal for PID %d\n", pid);
        for(int i = 0; i < current_job; i++) {
            if(jobs[i].pid == pid) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    strcpy(jobs[i].status, "Done");
                    printf("\n[%d]  Done\t\t%s\n", jobs[i].job_id, jobs[i].command);
                    clean_jobs(pid);
                } else if (WIFSTOPPED(status)) {
                    strcpy(jobs[i].status, "Stopped");
                    printf("\n[%d]  Stopped\t\t%s\n", jobs[i].job_id, jobs[i].command);
                } else if (WIFCONTINUED(status)) {
                    strcpy(jobs[i].status, "Running");
                } 
                break;
            }
        }
        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            printf ("The child exited.\n");
            printf("Debug: PID %d exited with status %d\n", pid, exit_status);
        } else if (WIFSIGNALED(status)) {
            exit_status = 128 + WTERMSIG(status);
            printf("The child was terminated.\n");
            printf("Debug: PID %d terminated by signal %d\n", pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            exit_status = 128 + WSTOPSIG(status);
            printf("The child was suspended.\n");
            printf("Debug: PID %d stopped by signal %d\n", pid, WSTOPSIG(status));
        }
    else
        printf ("Uninteresting\n");
    }
}

void SIGINTHandler(int sig) {
    if(pid_track > 0) {
        kill(-pid_track, SIGINT);
        printf("Child process %d killed.\n", pid_track);
    }
}

void SIGTSTPHandler(int sig) {
    if(pid_track > 0) {
        kill(-pid_track, SIGTSTP);
        printf("Child process %d suspended.\n", pid_track);
    }
}