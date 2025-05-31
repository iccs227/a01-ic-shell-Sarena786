/* ICCS227: Project 1: icsh
 * Name: Sareena Aulla
 * StudentID: 6481197
 */

#include <stdio.h>
#include "input.h"
#include "command.h"
#include "signal.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "command.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_BUFFER 255

pid_t shell_id;
int exit_status = 0;
volatile sig_atomic_t pid_fg = 0;

int main(int argc, char *argv[]) {

    shell_id = getpid();

    setpgid(0, 0);  // shell owns the process group
    tcsetpgrp(STDIN_FILENO, shell_id); // let shell controls the terminal

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGINT, forward_sigint);
    signal(SIGTSTP, forward_sigtstp);
    // ignore the signal in the shell

    signalHandler();

    printf("Shell PID: %d, PGID: %d\n", shell_id, getpgrp());

    FILE *fp = NULL;

    if(argc > 1) {
        fp = fopen(argv[1], "r"); // open the script file
        if(fp == NULL) {
            printf("Cannot Open the file");
            return -1;
        }
    }

    printf("Starting IC Shell\n");

    Run(fp);
    
    if(fp != NULL) {
        fclose(fp);
    }

    return exit_status;
}

/*
Start with
Milestone 1 : Interactive command-line interpreter
1. Initialize the shell (save shell PGID, ignore signals).
2. Start reading user input.
3. Parse and Execute commands (via cmdHandler).

Milestone 2 : Script mode
1. Detect if an input file is provided.
2. Read and execute commands from the file line by line.
3. Exit the shell when the file ends.

Milestone 3 : Running an external program in the foreground
1. Create a new child process.
2. In the child, run the external command (via execvp).
3. In the parent, wait for the child using waitpid.

Milestone 4 : Signal Handler
1. Set up signal handlers and register them in main.
2. Handle signals differently for shell, foreground, and background jobs.
3. Use exit status to detect job state (terminated and stopped)

Milestone 5 : I/O redirection
1. Detect '<' or '>' symbols in the command line.
2. Open the specified file and redirect STDIN or STDOUT.
3. Remove redirection symbols and file names from the args passed to execvp.
4. Close the file descriptor after redirection.

Milestone 6 : Background jobs and job control (HARD HARD HARD HARD)
1. T-T
2. Set shell to be its own process group, and ignore interactive signals.
3. In the child process:
    - Set a new process group ID.
    - If foreground: give it terminal control and restore default signal behavior.
4. In the parent process:
    - For foreground jobs: give terminal control to the child, wait for it, then take terminal back.
    - For background jobs: do not wait, just return the prompt and track the job.
5. Signal handler updates job status (done, stopped and running).

Background Jobs ('&')
- Detect '&' in the command.
- Run the command without waiting in the parent.
- Store job info (job_id, command, status, pid) in a 'job' struct.
- Parent immediately returns prompt.
- Signal handler prints status when job finishes or stops.

'jobs' command
- List all current jobs that are "Running" or "Stopped".

'fg %<id>'
- Implement 'to_fg(int id)':
    - Find the job with that job_id.
    - Give terminal control to the job.
    - Send SIGCONT to resume it.
    - Wait for it to finish or stop.
    - Return control back to the shell and update status via exit handler.

'bg %<id>'
- Implement 'cont_bg(int id)':
    - Find the stopped job with that job_id.
    - Change status to "Running".
    - Send SIGCONT to continue it in the background.

Signal in the background
- Ctrl+z -> Stop foreground job, can resume using 'bg %<id>'.
- Ctrl+c -> Terminate foreground job.

*/
