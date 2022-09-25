#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define MAX 1000

extern int fg_pid;
extern char *fgname;
extern int bg_processes[100000];
extern char **bg_procname;
extern int *bgno;
extern int bg_bitmaps[100000];

void ctrlc(int signum)
{
    if (fg_pid > 0)
    {
        int pid = tcgetpgrp(STDOUT_FILENO);
        kill(fg_pid, SIGINT);
        strcpy(fgname, "");
    }
}

void ctrlz(int signum)
{
    if (fg_pid > 0)
    {
        kill(fg_pid, SIGSTOP);
        setpgid(fg_pid, 0);
        bg_processes[*bgno] = fg_pid;
        bg_procname[*bgno] = (char *)malloc(sizeof(char *));

        char file[MAX] = "/proc/";
        char buffer[MAX];
        sprintf(buffer, "%d", fg_pid);
        strcat(file, buffer);
        strcat(file, "/stat");
        FILE *proc = fopen(file, "r");
        char status;
        long unsigned int memory;
        char *fg = (char *)malloc(sizeof(char *));
        if (fscanf(proc, "%*d %s", fg) == -1)
        {
            perror("scanf error");
            return 0;
        }
        strncpy(bg_procname[*bgno], &fg[1], strlen(fg) - 2);
        bg_bitmaps[*bgno] = 0;
        (*bgno)++;
        free(fg);
    }
}