#include <stdio.h>
#include <stdlib.h>

extern int bg_processes[100000];
extern char **bg_procname;

int fg(char **arr, int c)
{
    if (c != 2)
    {
        perror("Wrong number of parameters");
        return 0;
    }
    int jobno = atoi(arr[1]);

    if (jobno < 1 || bg_processes[jobno - 1] < 0)
    {
        perror("Process with given job number has already terminated");
        return 0;
    }
    
}