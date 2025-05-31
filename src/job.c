#include "job.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "command.h"
#include <sys/wait.h>
#include <errno.h>
#include "signal.h"

job jobs[MAX_JOBS];
int current_job = 0;
int job_id = 1;
extern volatile sig_atomic_t pid_fg;
extern pid_t shell_id;

void keepJob(pid_t pid, const char *command) {
    if (current_job >= MAX_JOBS) {
        printf("Maximum job limit reached.\n");
        return;
    }
    // Save job info in the jobs array
    jobs[current_job].pid = pid;                        // process ID
    jobs[current_job].job_id = job_id++;                // unique job ID
    strncpy(jobs[current_job].command, command, sizeof(jobs[current_job].command));     // Save command
    strncpy(jobs[current_job].status, "Running", sizeof(jobs[current_job].status));    
    printf("[%d] %d\n", jobs[current_job].job_id, pid);
    current_job++;
}

void printJobs() {
    for (int i = 0; i < current_job; i++) {
        if (strcmp(jobs[i].status, "Running") == 0 || strcmp(jobs[i].status, "Stopped") == 0) {
            printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id,jobs[i].status ,jobs[i].command);
        }
    }
}

void clean_jobs(pid_t pid) {
    for (int i = 0; i < current_job; i++) {
        if (jobs[i].pid == pid) {
            for (int j = i; j < current_job - 1; j++) { // Shift all jobs after this one to the left
                jobs[j] = jobs[j + 1];
            }
            current_job--;
            break;
        }
    }
}

void to_fg(int id) {
    pid_t pid;

    for(int i = 0; i < current_job; i++) {
        if (jobs[i].job_id == id) {
            if(strcmp(jobs[i].status, "Running") == 0 || strcmp(jobs[i].status, "Stopped") == 0) {
                pid = jobs[i].pid;
                strcpy(jobs[i].status, "Running");

                printf("%s\n", jobs[i].command);
                
                tcsetpgrp(STDIN_FILENO, pid);   // Give the terminal control to the job's process group
                kill(-pid, SIGCONT);            // Send signal (to continue) to the whole process group

                pid_fg = pid;

                int status;
                waitpid(pid, &status, WUNTRACED); 

                tcsetpgrp(STDIN_FILENO, shell_id); // back to the shell, once done.

                pid_fg = 0;

                exit_handler(status, pid, i);

                }
                break;
            } else {
                printf("Job %d not found.\n", id);
            }
        }
    }

void cont_bg(int id) {
    pid_t pid;
    for (int i = 0; i < current_job; i++) {
        if (jobs[i].job_id == id && strcmp(jobs[i].status, "Stopped") == 0) {
            pid = jobs[i].pid;
            strcpy(jobs[i].status, "Running");
            printf("\n[%d]  %s\t\t%s\n", jobs[i].job_id, jobs[i].status, jobs[i].command);

            kill(-pid, SIGCONT);

            return;
        } else {
            printf("Job [%d] not found.\n", id);
        }
    }
}