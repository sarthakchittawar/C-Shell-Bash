#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX 1000

extern int fg_pid;

void ctrlc(int signum)
{
    if (fg_pid > 0)
    {
        int pid = tcgetpgrp(STDOUT_FILENO);
        kill(fg_pid, SIGINT);
    }
}