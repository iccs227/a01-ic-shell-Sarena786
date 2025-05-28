#include <sys/types.h>
#ifndef JOB_H
#define JOB_H

#define MAX_JOBS 256

typedef struct {
    pid_t pid;
    int job_id;
    char status[16];
    char command[256];
} job;

extern job jobs[MAX_JOBS];
extern int current_job; 
extern int job_id;

void keepJob(pid_t pid, const char *command);

#endif