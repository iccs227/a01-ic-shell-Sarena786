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
    printf("Debug: Added job [%d] with PID %d, command: %s\n", jobs[current_job].job_id, pid, command);
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

void clean_jobs(pid_t pid) {
    for (int i = 0; i < current_job; i++) {
        if (jobs[i].pid == pid) {
            printf("Debug: Cleaning job [%d] with PID %d\n", jobs[i].job_id, pid);
            for (int j = i; j < current_job - 1; j++) {
                jobs[j] = jobs[j + 1];
            }
            current_job--;
            break;
        }
    }
}

void to_fg(int id) {
    int found = 0;
    pid_t pid;

    for(int i = 0; i < current_job; i++) {
        if (jobs[i].job_id == id) {
            found = 1;

            if(strcmp(jobs[i].status, "Running") == 0 || strcmp(jobs[i].status, "Stopped") == 0) {
                pid = jobs[i].pid;
                strcpy(jobs[i].status, "Running");
                printf("%s\n", jobs[i].command);

                printf("Debug: Bringing job %d to foreground\n", id);
                printf("Debug: Sending SIGCONT to PID %d\n", pid);
                
                tcsetpgrp(STDIN_FILENO, pid);
                kill(-pid, SIGCONT);

                pid_track = pid;

                int status;
                waitpid(pid_track, &status, WUNTRACED);

                tcsetpgrp(STDIN_FILENO, shell_id);

                pid_track = -1;

                if(WIFSTOPPED(status)) {
                    strcpy(jobs[i].status, "Stopped");
                    printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
                } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    strcpy(jobs[i].status, "Done");
                    printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);
                    clean_jobs(pid);
                }
            }
            break;
        }
    }
    if(!found) {
        printf("Job not found.\n");
    }
}

void cont_bg(int id) {
    for (int i = 0; i < current_job; i++) {
        if (jobs[i].job_id == id && strcmp(jobs[i].status, "Stopped") == 0) {
            strcpy(jobs[i].status, "Running");
            kill(-jobs[i].pid, SIGCONT);
            printf("[%d]+ %s &\n", jobs[i].job_id, jobs[i].command);
            return;
        }
    }
}