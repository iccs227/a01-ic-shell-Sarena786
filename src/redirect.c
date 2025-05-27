#include<stdio.h>
#include <fcntl.h>
#include "input.h"
#include<string.h>
#include <unistd.h>

void redirect(char **args) {
    int in = -1;
    int out = -1;
    int j = 0;
    for(int i = 0; args[i] != NULL; i++) {
        if(strcmp(args[i], ">") == 0) {
            out = open(args[i+1], O_TRUNC | O_CREAT | O_WRONLY, 0666);
            dup2(out,1);
            i++;
        } else if (strcmp(args[i], "<") == 0) {
            in = open (args[i+1], O_RDONLY);
            dup2(in, 0);
            i++;
        } else {
            args[j++] = args[i];
        }
    }
    args[j] = NULL;
    if(in != -1) {close(in);}
    if(out != -1) {close(out);}
}