#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

extern int bg_processes[100000];
extern int bg_bitmaps[100000];

int bg(char **arr, int c)
{
    if (c != 2)
    {
        perror("Wrong number of arguments");
        return 0;
    }
    int index = atoi(arr[1]) - 1;
    int pid = bg_processes[index];
    if (pid < 0)
    {
        perror("Process with given job number has already terminated");
        return 0;
    }

    kill(pid, SIGCONT);

    return 1;
}