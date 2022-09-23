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
#include "trim.h"
#include "check_dir.h"
#include "check_path.h"
#include "give_path.h"
#include "ls.h"
#include "history.h"
#include "add_hist.h"
#include "pinfo.h"
#include "handler.h"
#include "others.h"
#include "process.h"
#include "take_input.h"

#define MAX 1000

extern int errno;

char prev_dir[MAX];
int bgcount = 0;
int bg_processes[100000];
int bg_bitmaps[100000];
char bg_buffer[MAX];
int andflag;
int handlerflag = 0;
char **bg_procname;

int main()
{
    bg_procname = (char **)malloc(MAX * __SIZEOF_POINTER__);
    for (int i = 0; i < 100000; i++)
    {
        bg_bitmaps[i] = 0;
    }

    FILE *histfile = fopen(".history", "r");
    if (histfile == NULL)
    {
        FILE *histfile2 = fopen(".history", "w");
        fclose(histfile2);
    }
    else
    {
        fclose(histfile);
    }

    double bef, en;
    char sysname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    gethostname(sysname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);

    char prompt[MAX] = "<";
    strcat(prompt, username);
    strcat(prompt, "@");
    strcat(prompt, sysname);

    char curr_dir[MAX];
    if (getcwd(curr_dir, MAX) == NULL)
    {
        perror("Could not get current directory");
    }

    char dir[MAX] = "~";
    char init_dir[MAX];
    strcpy(init_dir, curr_dir);
    int timeflag = 0;

    char temp[MAX];
    strcpy(temp, init_dir);
    FILE *fp = fopen(strcat(temp, "/.history"), "r+"); // file guaranteed to exist
    char **history = (char **)malloc(MAX * 20);
    for (int i = 0; i < 20; i++)
    {
        history[i] = (char *)malloc(MAX);
    }
    int histno = 0;
    while (histno < 20 && fgets(history[histno], MAX, fp))
    {
        histno++;
    }

    signal(SIGCHLD, handler);

    while (1)
    {
        if (timeflag == 0)
        {
            printf("\033[1;32m");
            printf("%s", prompt);
            printf("\033[1;37m");
            printf(":");
            printf("\033[1;34m");
            printf("%s> ", dir);
            printf("\033[0;37m");
        }
        else
        {
            printf("\033[1;32m");
            printf("%s", prompt);
            printf("\033[1;37m");
            printf(":");
            printf("\033[1;34m");
            time_t round = lroundf(en - bef);
            printf("%stook %lds> ", dir, round);
            printf("\033[0;37m");
            timeflag = 0;
        }

        char text[MAX];
        memset(text, '\0', MAX);

        take_input(text);
        if (add_hist(&histno, text, history) == 0)
        {
            continue;
        }

        fseek(fp, 0, SEEK_SET);
        for (int i = 0; i < histno; i++)
        {
            fprintf(fp, "%s", history[i]);
        }
        fflush(fp);

        char **arr = (char **)malloc(MAX * __SIZEOF_POINTER__);
        int c = 0;
        char *token = strtok(text, ";");
        while (token != NULL)
        {
            arr[c] = token;
            c++;
            token = strtok(NULL, ";");
        }
        // make array and store
        int d = 0;
        while (d < c)
        {
            trim(arr[d]);
            // create process
            if (strcmp(arr[d], "") == 0)
            {
                break;
            }
            int andcount = 0;
            int pipecount = 0;

            for (int i = 0; i < strlen(arr[d]); i++)
            {
                if (arr[d][i] == '&')
                {
                    andcount++;
                }
                else if (arr[d][i] == '|')
                {
                    pipecount++;
                }
            }
            char **arr1 = (char **)malloc(MAX * __SIZEOF_POINTER__);
            char *token1 = strtok(arr[d], "|");
            int e = 0;
            while (token1 != NULL)
            {
                trim(token1);
                arr1[e] = token1;
                e++;
                token1 = strtok(NULL, "|");
            }
            for (int f = 0; f < e; f++)
            {
                char *token2 = strtok(arr1[f], "&");
                char **arr2 = (char **)malloc(MAX * __SIZEOF_POINTER__);
                int a = 0;
                while (token2 != NULL)
                {
                    arr2[a] = token2;
                    a++;
                    token2 = strtok(NULL, "&");
                }
                int b = 0;
                andflag = 0;
                while (b < a)
                {
                    trim(arr2[b]);

                    if (strcmp(arr2[b], "") == 0)
                    {
                        break;
                    }
                    if (b < andcount)
                    {
                        andflag = 1;
                    }
                    else
                    {
                        andflag = 0;
                    }

                    process(arr2[b], andflag, dir, curr_dir, init_dir, prev_dir, username, &timeflag, prompt, history, histno, &bgcount, bg_processes, bg_procname, bg_bitmaps, &bef, &en);
                    b++;
                }
                // pipelining
                if (f != e - 1)
                {
                    int filedes[2];
                    if (pipe(filedes) < 0)
                    {
                        perror("Could not pipe correctly");
                        return 0;
                    }
                }
            }
            // int w = 0;
            // wait(&w);

            d++;
        }
        free(arr);
        memset(text, 0, sizeof(text));
        if (strcmp(bg_buffer, "") != 0)
        {
            fprintf(stderr, "%s", bg_buffer);
        }
        memset(bg_buffer, '\0', MAX);
    }
    fclose(fp);

    return 0;
}