#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX 1000

extern int bg_processes[100000];
extern char **bg_procname;
extern int fg_pid;
extern char *fgname;

int fg(char **arr, int c, int *timeflag, double *bef, double *en)
{
    if (c != 2)
    {
        perror("Wrong number of parameters");
        return 0;
    }
    int jobno = atoi(arr[1]);

    if (jobno < 1 || bg_processes[jobno - 1] < 0)
    {
        perror("Process with given job number does not exist or has already terminated");
        return 0;
    }
    int pid = bg_processes[jobno - 1];
    bg_processes[jobno - 1] = -1;
    kill(pid, SIGCONT);
    int pgid = getpid();
    setpgid(pid, pgid);
    struct timeval begin, end;

    gettimeofday(&begin, NULL);

    fg_pid = pid;
    char procname[MAX] = "";
    trim(procname);
    if (strcmp(arr[0], "fg") != 0)
    {
        strcpy(fgname, procname);
    }
    waitpid(pid, NULL, WUNTRACED);
    fg_pid = -1;

    gettimeofday(&end, NULL);
    (*bef) = begin.tv_sec + begin.tv_usec / (double)1000000;
    (*en) = end.tv_sec + end.tv_usec / (double)1000000;

    if ((*en) - (*bef) >= 1)
    {
        *timeflag = 1;
    }
}