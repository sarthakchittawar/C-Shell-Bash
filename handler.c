#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>

#define MAX 1000

extern int bgcount;
extern int bg_processes[100000];
extern char bg_buffer[MAX];
extern int andflag;

void handler(int signum)
{
    if (andflag == 0)
    {
        return;
    }
    int status;
    int pid = wait(&status);
    
    int num = -1;

    for (int i = 0; i < bgcount; i++)
    {
        if (bg_processes[i] == pid)
        {
            bg_processes[i] = -1;
            num = i + 1;
            break;
        }
    }
    if (num == -1)
    {
        return;
    }

    if (pid > 0)
    {
        char str[MAX];
        sprintf(str, "\r[%d] with pid %d exited", num, pid);
        strcat(bg_buffer, str);
        
        int exit;
        memset(str, '\0', MAX);

        if (WIFEXITED(status))
        {
            exit = WEXITSTATUS(status);
            sprintf(str, " normally with exit status %d\n", exit);
        }
        else
        {
            sprintf(str, " abnormally\n");
        }
        strcat(bg_buffer, str);
    }
    else
    {
        perror("PID cannot be <=0");
    }
    return;
}