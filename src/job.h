#include <sys/types.h>
#ifndef JOB_H
#define JOB_H

#define MAX_JOBS 256

typedef struct {
    int pid;
    int job_id;
    char status[16];
    char command[256];
} job;

extern job jobs[MAX_JOBS];
extern int current_job; 
extern int job_id;
extern job* fg_job;

void keepJob(int pid, const char *command);
void printJobs();
void to_fg(int id);
void cont_bg(int id);
void clean_jobs(pid_t pid);

#endif