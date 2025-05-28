#include "job.h"
#include<stdio.h>

job jobs[MAX_JOBS];
int current_job = 0;
int job_id = 1;

void keepJob(int pid, const char *command) {
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

void cmdJobs() {
    for(int i = 0; i < current_job; i++){
        if(strcmp(jobs[current_job].status, "Running") == 0) {
            printf("[%d]  Running\t\t%s\n", jobs[i].job_id, jobs[i].command);
        }
    }
}