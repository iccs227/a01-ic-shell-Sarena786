#include<stdio.h>
#include <fcntl.h>
#include "input.h"

int in;
int out;
size_t got;
char buffer[1024];

void redirect(char **args) {
    for(int i = 0; args[i] != NULL; i++) {
        if(strcmp(args[i], ">") == 0) {
            out = open(args[i+1], O_TRUNC | O_CREAT | O_WRONLY, 0666);
            if(out < 0) {
                l
            }
            dup2(out,1);
        } else if (strcmp(args[i], "<") == 0) {
            in = open (args[i+1], O_RDONLY);
            dup2(in, 0);
        }
    }
    close(in);
    close(out);
}