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

#include "give_path.h"

#define MAX 1000

int discfunc(int *flag, char *target_dir, int d, int f, char *file_name, char *curr_dir, char *init_dir)
{
    if (strcmp(file_name, "") == 0)
    {
        // contents of target dir
        char path[MAX];
        if (give_path(curr_dir, init_dir, target_dir, path) != NULL)
        {
            struct dirent **array;
            int num;
            if ((num = scandir(path, &array, NULL, alphasort)) != -1)
            {
                if (d > 0 || (d == 0 && f == 0))
                {
                    printf("%s\n", target_dir);
                }
                for (int i = 0; i < num; i++)
                {
                    if (strcmp(array[i]->d_name, ".") != 0 && strcmp(array[i]->d_name, "..") != 0)
                    {
                        char new_dir[MAX];
                        strcpy(new_dir, target_dir);
                        strcat(new_dir, "/");
                        strcat(new_dir, array[i]->d_name);

                        discfunc(flag, new_dir, d, f, file_name, curr_dir, init_dir);
                    }
                }
            }
            else if (f > 0 || (d == 0 && f == 0))
            {
                printf("%s\n", target_dir);
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        char path[MAX];
        if (give_path(curr_dir, init_dir, target_dir, path) != NULL)
        {
            struct dirent **array;
            int num;
            if ((num = scandir(path, &array, NULL, alphasort)) != -1)
            {
                if (d > 0 || (d == 0 && f == 0))
                {
                    if (strcmp(file_name, basename(target_dir)) == 0)
                    {
                        printf("%s\n", target_dir);
                        (*flag) = 1;
                        // return 1;
                    }
                }
                for (int i = 0; i < num; i++)
                {
                    if (strcmp(array[i]->d_name, ".") != 0 && strcmp(array[i]->d_name, "..") != 0)
                    {
                        char new_dir[MAX];
                        strcpy(new_dir, target_dir);
                        strcat(new_dir, "/");
                        strcat(new_dir, array[i]->d_name);

                        discfunc(flag, new_dir, d, f, file_name, curr_dir, init_dir);
                        // if ((*flag) == 1)
                        // {
                        //     return 1;
                        // }
                    }
                }
            }
            else if (f > 0 || (d == 0 && f == 0))
            {
                if (strcmp(file_name, basename(target_dir)) == 0)
                {
                    printf("%s\n", target_dir);
                    (*flag) = 1;
                    // return 1;
                }
            }
        }
        if ((*flag) == 0)
        {
            return 0;
        }
    }
}

int discover(int c, char **arr, char *curr_dir, char *init_dir)
{
    char target_dir[MAX];
    memset(target_dir, '\0', MAX);
    int d = 0, f = 0;
    char file_name[MAX];
    memset(file_name, '\0', MAX);
    int dirno = 0;
    int fileno = 0;

    for (int i = 1; i < c; i++)
    {
        if (arr[i][0] == '-')
        {
            for (int j = 1; j < strlen(arr[i]); j++)
            {
                if (arr[i][j] == 'd')
                {
                    d++;
                }
                else if (arr[i][j] == 'f')
                {
                    f++;
                }
                else
                {
                    perror("Invalid Flag(s)");
                    return 0;
                }
            }
        }
        else if (arr[i][0] == '"')
        {
            strncpy(file_name, &arr[i][1], strlen(arr[i]) - 2);
            fileno++;
        }
        else
        {
            strcpy(target_dir, arr[i]);
            dirno++;
        }
    }
    if (strcmp(target_dir, "") == 0)
    {
        strcpy(target_dir, ".");
    }
    int flag = 0;
    if (dirno > 1 || fileno > 1)
    {
        perror("Invalid Parameters");
        return 0;
    }
    if (discfunc(&flag, target_dir, d, f, file_name, curr_dir, init_dir) == 0)
    {
        perror("No such filename or directory");
        return 0;
    }
    return 1;
}