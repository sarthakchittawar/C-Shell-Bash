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
    char prname[MAX];
    strcpy(prname, process_name);

    char *token = strtok(process_name, "\t ");
    char **arr = (char **)malloc(MAX * __SIZEOF_POINTER__);
    int c = 0;
    while (token != NULL)
    {
        arr[c] = token;
        c++;
        token = strtok(NULL, "\t ");
    }
    int inflag = 0, outflag = 0, in_backup, out_backup, in_index = -1, out_index = -1;
    FILE *fp;
    for (int i = 0; i < c; i++)
    {
        if (i != c - 1 && strcmp(arr[i], "<") == 0)
        {
            inflag = 1;
            in_backup = dup(STDIN_FILENO);
            char path[MAX];
            if ((fp = fopen(arr[i + 1], "r")) != NULL)
            {
                dup2(fileno(fp), STDIN_FILENO);
                in_index = i;
            }
            else
            {
                return 0;
            }
        }
        else if (i != c - 1 && strcmp(arr[i], ">") == 0)
        {
            outflag = 1;
            dup2(STDOUT_FILENO, out_backup);
            if ((fp = fopen(arr[i + 1], "w")) != NULL)
            {
                dup2(fileno(fp), STDOUT_FILENO);
                out_index = i;
            }
            else
            {
                if (open(arr[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0)
                {
                    perror("Failed to open file");
                    return 0;
                }
                fp = fopen(arr[i + 1], "r");

                dup2(fileno(fp), STDOUT_FILENO);
                out_index = i;
            }
        }
        else if (i != c - 1 && strcmp(arr[i], ">>") == 0)
        {
            outflag = 2;
            dup2(STDOUT_FILENO, out_backup);
            if ((fp = fopen(arr[i + 1], "a")) != NULL)
            {
                dup2(fileno(fp), STDOUT_FILENO);
                out_index = i;
            }
            else
            {
                if (open(arr[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0)
                {
                    perror("Failed to open file");
                    return 0;
                }
                fp = fopen(arr[i + 1], "a");

                dup2(fileno(fp), STDOUT_FILENO);
                out_index = i;
            }
        }
    }

    char *tok = strtok(prname, "\t ");
    c = 0;
    while (tok != NULL)
    {
        if (inflag == 0 && outflag == 0)
        {
            arr[c] = tok;
            c++;
        }
        else if (inflag == 0 && outflag != 0)
        {
            if (c != out_index && c != out_index + 1)
            {
                arr[c] = tok;
                c++;
            }
        }
        else if (inflag != 0 && outflag == 0)
        {
            if (c != in_index && c != in_index + 1)
            {
                arr[c] = tok;
                c++;
            }
        }
        else if (inflag != 0 && outflag != 0)
        {
            if (c != out_index && c != out_index + 1 && c != in_index && c != in_index + 1)
            {
                arr[c] = tok;
                c++;
            }
        }
        tok = strtok(NULL, "\t ");
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
            fclose(proc);
        }
        if (flag == 0 && strcmp(arr[0], "pinfo") != 0) // exception
        {
            (*bgcount) = 0;
        }
    }

    // if (inflag != 0 && outflag != 0)
    // {
    //     dup2(STDOUT_FILENO, STDIN_FILENO);
    // }

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
            dup2(in_backup, STDIN_FILENO);
            dup2(out_backup, STDOUT_FILENO);
            return 0;
        }
        free(arr);
    }
    else if (strcmp(arr[0], "ls") == 0)
    {
        if (ls_main(process_name, dir, curr_dir, init_dir, prev_dir, username, arr, c) == 0)
        {
            free(arr);
            dup2(in_backup, STDIN_FILENO);
            dup2(out_backup, STDOUT_FILENO);
            return 0;
        }
        free(arr);
    }
    else if (strcmp(arr[0], "history") == 0)
    {
        if (history(c, hist, histno) == 0)
        {
            free(arr);
            dup2(in_backup, STDIN_FILENO);
            dup2(out_backup, STDOUT_FILENO);
            return 0;
        }
        free(arr);
    }
    else if (strcmp(arr[0], "pinfo") == 0)
    {
        if (pinfo(c, arr, curr_dir, bg_processes, *bgcount) == 0)
        {
            free(arr);
            dup2(in_backup, STDIN_FILENO);
            dup2(out_backup, STDOUT_FILENO);
            return 0;
        }
        free(arr);
    }
    else if (strcmp(arr[0], "discover") == 0)
    {
        if (discover(c, arr, curr_dir, init_dir) == 0)
        {
            free(arr);
            dup2(in_backup, STDIN_FILENO);
            dup2(out_backup, STDOUT_FILENO);
            return 0;
        }
        free(arr);
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
    dup2(in_backup, STDIN_FILENO);
    dup2(out_backup, STDOUT_FILENO);

    return 1;
}