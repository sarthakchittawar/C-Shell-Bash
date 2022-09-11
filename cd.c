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
#include <dirent.h>
#include <time.h>

#include "trim.h"
#include "check_dir.h"
#include "give_path.h"

#define MAX 1000

int cd(char *process_name, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, char **arr, int c)
{
    if (c == 1)
    {
        strcpy(dir, "~");
        strcpy(curr_dir, init_dir);
        chdir(init_dir);
    }
    else if (c > 2)
    {
        perror("Error, too many arguments");
        return 0;
    }

    if (c == 2)
    {
        if (!check_dir(curr_dir, init_dir, arr[1]))
        {
            perror("No directory present with such name");
            return 0;
        }
        char tt[MAX];
        give_path(curr_dir, init_dir, arr[1], tt);
        if (chdir(tt) == -1)
        {
            perror("Could not change directory");
            return 0;
        }

        char prev[MAX];
        strcpy(prev, prev_dir);
        strcpy(prev_dir, curr_dir);

        char *t = (char *)malloc(MAX);
        strcpy(t, arr[1]);
        char *token2 = strtok(t, "/");
        char **array = (char **)malloc(MAX * __SIZEOF_POINTER__);
        int ind = 0;

        while (token2 != NULL)
        {
            array[ind] = token2;
            trim(token2);
            token2 = strtok(NULL, "/");
            ind++;
        }
        int max = ind;
        ind = 0;
        while (c == 2 && ind < max)
        {
            token2 = array[ind];
            if (strcmp(token2, "..") == 0)
            {
                char *d = dirname(curr_dir);
                strcpy(curr_dir, d);
                if (strcmp(d, init_dir) == 0)
                {
                    strcpy(dir, "~");
                }
                else
                {
                    strcpy(dir, d);
                }
            }
            else if (strcmp(token2, "~") == 0)
            {
                strcpy(curr_dir, init_dir);
                strcpy(dir, "~");
            }
            else if (strcmp(token2, "-") == 0)
            {
                if (prev[0] == '\0')
                {
                    perror("No previous directory");
                    break;
                }
                strcpy(curr_dir, prev);
                if (strcmp(curr_dir, init_dir) == 0)
                {
                    strcpy(dir, "~");
                }
                else
                {
                    strcpy(dir, curr_dir);
                }
                printf("%s\n", curr_dir);
            }
            else
            {
                if (strcmp(token2, ".") == 0)
                {
                    token2 = strtok(NULL, "/");
                    if (token2 != NULL)
                    {
                        trim(token2);
                    }
                    ind++;
                    continue;
                }
                strcpy(prev_dir, curr_dir);
                strcpy(prev, prev_dir);
                if (token2[0] != '/')
                {
                    strcat(curr_dir, "/");
                }

                strcat(curr_dir, token2);
                if (strcmp(curr_dir, init_dir) == 0)
                {
                    strcpy(dir, "~");
                }
                else
                {
                    strcpy(dir, curr_dir);
                }
            }
            ind++;
        }
        free(t);
        free(array);
    }
    if (strncmp(init_dir, curr_dir, strlen(init_dir)) == 0)
    {
        strcpy(dir, "~");
        char temp[MAX];
        int i;
        for (i = strlen(init_dir); i < strlen(curr_dir); i++)
        {
            temp[i - strlen(init_dir)] = curr_dir[i];
        }
        temp[i - strlen(init_dir)] = '\0';

        strcat(dir, temp);
    }
}