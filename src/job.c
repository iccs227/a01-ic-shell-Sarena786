#include "job.h"

job jobs[MAX_JOBS];
extern int current_job = 0;
extern int job_id = 1;

void keepJob(pid_t pid, const char *command) {
    jobs[current_job].pid = pid;
    jobs[current_job].job_id = job_id++;
    
}