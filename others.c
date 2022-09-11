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

void others(char **arr, int c, char *process_name, int andflag, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, int *timeflag, char *prompt, char **hist, int histno, int *bgcount, int bg_processes[], double *bef, double *en)
{
    if (andflag == 0)
    {
        pid_t pid;
        struct timeval begin, end;

        gettimeofday(&begin, NULL);

        pid = fork();
        if (pid == 0)
            {
                arr[c] = NULL;
                if (execvp(arr[0], arr) < 0)
                {
                    perror("Invalid command/ not coded yet");
                }
            }
        else
        {
            wait(&pid);
        }
        gettimeofday(&end, NULL);
        (*bef) = begin.tv_sec + begin.tv_usec / (double)1000000;
        (*en) = end.tv_sec + end.tv_usec / (double)1000000;

        if ((*en) - (*bef) >= 1)
        {
            *timeflag = 1;
        }
    }
    else
    {
        pid_t pid;

        if ((pid = fork()) == 0)
        {
            arr[c] = NULL;
            if (execvp(arr[0], arr) < 0)
            {
                perror("Invalid command/ not coded yet");
            }
        }
        else
        {
            bg_processes[*bgcount] = pid;
            printf("[%d] %d\n", (*bgcount) + 1, pid);

            (*bgcount)++;
        }
    }
}