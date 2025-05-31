[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WIXYXthJ)
# ICSH

Please describe your assumptions and/or implementation here.

## What I’ve done so far:

### Milestone 1: Basic shell
- Start the shell, save its process group ID.
- Ignore signals like Ctrl+C in the shell itself.
- Read commands from the user and run them.

### Milestone 2: Script mode
- If a script file is passed as input, read and run commands from the file.
- Exit when the script ends.

### Milestone 3: Running external commands
- Fork a child process to run commands with `execvp`.
- Parent waits for the child to finish.

### Milestone 4: Signal handling
- Setup signal handlers.
- Handle signals differently for the shell, foreground jobs, and background jobs.
- Track job states (running, stopped, done).

### Milestone 5: Input/output redirection
- Detect `<` and `>` in commands.
- Redirect stdin/stdout to files.
- Remove redirection symbols before executing commands.

### Milestone 6: Background jobs and job control (hard stuff!)
- Make the shell its own process group and ignore signals.
- Child process sets its own group, handles signals properly.
- Parent process waits for foreground jobs, doesn’t wait for background jobs.
- Track background jobs with a job list (job ID, command, status, pid).
- Support `&` for background jobs.
- Implement `jobs`, `fg %id`, and `bg %id`.
- Ctrl+Z stops the foreground job; Ctrl+C kills it.

---

## Some notes
## What I Learned About Process Groups and Signals

- At first, I set the shell’s process group ID without ignoring signals first.  
- This made the shell crash or quit when I pressed Ctrl+C or Ctrl+Z.  
- Also, when running background jobs, sometimes the shell kept looping unexpectedly.  

What helped me fix it:

- First, make sure the shell owns the terminal by setting the shell’s process group.  
- Then, ignore signals like Ctrl+C and Ctrl+Z in the shell’s main process so the shell doesn’t quit.  
- When I create a child process to run a command, I restore the default signal handlers in that child.  
- This way, signals like Ctrl+C and Ctrl+Z work correctly for the running command, not the shell itself.

Signal things are tricky for me. T-T