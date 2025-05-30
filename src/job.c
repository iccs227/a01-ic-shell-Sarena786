#include "job.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "command.h"
#include <sys/wait.h>
#include <errno.h>

job jobs[MAX_JOBS];
int current_job = 0;
int job_id = 1;
job* fg_job = NULL;

void keepJob(pid_t pid, const char *command) {
    if (current_job >= MAX_JOBS) {
        printf("Maximum job limit reached.\n");
        return;
    }
    jobs[current_job].pid = pid;
    jobs[current_job].job_id = job_id++;
    strncpy(jobs[current_job].command, command, sizeof(jobs[current_job].command));
    strncpy(jobs[current_job].status, "Running", sizeof(jobs[current_job].status));
    printf("[%d] %d\n", jobs[current_job].job_id, pid);
    current_job++;
}

void printJobs() {
    for (int i = 0; i < current_job; i++) {
        if (strcmp(jobs[i].status, "Running") == 0 || strcmp(jobs[i].status, "Stopped") == 0) {
            printf("[%d] %-10s %s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
        }
    }
}

void to_fg(int id) {
    int found = -1; 

    for(int i = 0; i < current_job; i++) {
        if (jobs[i].job_id == id) {
            found = 1;
            if(strcmp(jobs[i].status, "Running") == 0 || strcmp(jobs[i].status, "Stopped")) {
                pid_track = jobs[i].pid;
                strcpy(jobs[i].status, "Running");
                printf("%s\n", jobs[i].command);
                kill(-pid_track, SIGCONT);
                waitpid(pid_track, NULL, WUNTRACED);
            // fg_job = &jobs[i];
            }
            break;
        }
    }
    if(!found) {
        printf("Job not found.\n");
    }
}