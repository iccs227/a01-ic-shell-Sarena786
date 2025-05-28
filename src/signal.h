#ifndef SIGNAL_H
#define SIGNAL_H
#include <signal.h>

void signalHandler();
void ChildHandler(int sig, siginfo_t *sip, void *notused);
void SIGINTHandler(int sig);
void SIGTSTPHandler(int sig);

extern int exit_status;

#endif