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

#include "cd.h"
#include "ls.h"
#include "history.h"
#include "pinfo.h"
#include "discover.h"
#include "others.h"

#define MAX 1000

int process(char *process_name, int andflag, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, int *timeflag, char *prompt, char **hist, int histno, int *bgcount, int bg_processes[], double *bef, double *en)
{
    char *token = strtok(process_name, "\t ");
    char **arr = (char **)malloc(MAX * __SIZEOF_POINTER__);
    int c = 0;
    while (token != NULL)
    {
        arr[c] = token;
        c++;
        token = strtok(NULL, "\t ");
    }

    // to reset bgcount
    if (andflag == 0)
    {
        int flag = 0;
        for (int i = 0; i < *bgcount; i++)
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
                    if (status == 'R')
                    {
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if (flag == 0 && strcmp(arr[0], "pinfo") != 0) // exception
        {
            (*bgcount) = 0;
        }
    }

    if (strcmp(arr[0], "pwd") == 0)
    {
        printf("%s\n", curr_dir);
    }
    else if (strcmp(arr[0], "echo") == 0)
    {
        for (int i = 1; i < c; i++)
        {
            printf("%s ", arr[i]);
        }
        printf("\n");
    }
    else if (strcmp(arr[0], "cd") == 0)
    {
        if (cd(process_name, dir, curr_dir, init_dir, prev_dir, username, arr, c) == 0)
        {
            free(arr);
            return 0;
        }
        free(arr);
        return 1;
    }
    else if (strcmp(arr[0], "ls") == 0)
    {
        if (ls_main(process_name, dir, curr_dir, init_dir, prev_dir, username, arr, c) == 0)
        {
            free(arr);
            return 0;
        }
        free(arr);
        return 1;
    }
    else if (strcmp(arr[0], "history") == 0)
    {
        if (history(c, hist, histno) == 0)
        {
            free(arr);
            return 0;
        }
        free(arr);
        return 1;
    }
    else if (strcmp(arr[0], "pinfo") == 0)
    {
        if (pinfo(c, arr, curr_dir, bg_processes, *bgcount) == 0)
        {
            free(arr);
            return 0;
        }
        free(arr);
        return 1;
    }
    else if (strcmp(arr[0], "discover") == 0)
    {
        if (discover(c, arr, curr_dir, init_dir) == 0)
        {
            free(arr);
            return 0;
        }
        free(arr);
        return 1;
    }
    else if (strcmp(arr[0], "exit") == 0)
    {
        free(arr);
        exit(0);
    }
    else
    {
        others(arr, c, process_name, andflag, dir, curr_dir, init_dir, prev_dir, username, timeflag, prompt, hist, histno, bgcount, bg_processes, bef, en);
        free(arr);
    }
}