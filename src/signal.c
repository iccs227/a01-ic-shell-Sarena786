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
                }
                break;
            }
        }
    }
}