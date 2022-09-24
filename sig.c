#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

extern int bg_processes[100000];

int sig(char **arr, int c)
{
    if (c != 3)
    {
        perror("Wrong number of arguments");
        return 0;
    }
    int index = atoi(arr[1]) - 1;
    int pid = bg_processes[index];
    if (pid < 0)
    {
        perror("Process with given job number does not exist or has already terminated");
        return 0;
    }
    int signum = atoi(arr[2]);
    if (signum <= 0 || signum > 31)
    {
        perror("Wrong signal number");
        return 0;
    }

    kill(pid, signum);

    return 1;
}