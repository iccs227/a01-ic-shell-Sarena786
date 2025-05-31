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
extern int exit_status;

void signalHandler() {

    struct sigaction action;
    action.sa_sigaction = ChildHandler;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &action, NULL);
}

void ChildHandler(int sig, siginfo_t *sip, void *notused) {
    int status;
    pid_t pid;
    fflush (stdout);


    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        for(int i = 0; i < current_job; i++) {
            if(jobs[i].pid == pid) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    exit_status = WEXITSTATUS(status);
                    strcpy(jobs[i].status, "Done");
                    printf("\n[%d]  Done\t\t%s\n", jobs[i].job_id, jobs[i].command);
                    clean_jobs(pid);
                }
                break;
            }
        }
    }
}

void exit_handler(int status, pid_t pid, int i) {
    if (WIFSTOPPED(status) ) {
        exit_status = 128 + WSTOPSIG(status);
        if (i >= 0) {
            strcpy(jobs[i].status, "Stopped");
            printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
        } else {
        printf("The process %d is suspended.\n", pid);
        }
    } else if (WIFEXITED(status)) {
        exit_status = WEXITSTATUS(status);
        if (i >= 0) {
            strcpy(jobs[i].status, "Done");
            printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
            clean_jobs(pid);
        } else {
        printf("The process %d exited.\n", pid);
        }
    } else if (WIFSIGNALED(status)) {
        exit_status = 128 + WTERMSIG(status);  // terminated by a signal
        if (i >= 0) {
            strcpy(jobs[i].status, "Done");
            printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
            clean_jobs(pid);
        } else {
        printf("The process %d was terminated.\n", pid);
        }
    }
} 