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
extern int bg_bitmaps[100000];
extern char bg_buffer[MAX];
extern int andflag;

void handler(int signum)
{
    // if (andflag == 0)
    // {
    //     return;
    // }
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    int num = -1;

    for (int i = 0; i < bgcount; i++)
    {
        if (bg_processes[i] == pid)
        {
            char file[MAX] = "/proc/";
            char buffer[MAX];
            sprintf(buffer, "%d", bg_processes[i]);
            strcat(file, buffer);
            strcat(file, "/stat");
            FILE *proc = fopen(file, "r");
            char status;
            long unsigned int memory;
            int fg;
            if (proc != NULL)
            {
                if (fscanf(proc, "%*d %*s %c", &status))
                {
                    if (status == 'T')
                    {
                        bg_bitmaps[i] = 0;
                    }
                }
                fclose(proc);
            }
            else
            {
                bg_processes[i] = -1;
            }
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