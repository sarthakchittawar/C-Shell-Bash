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

#define MAX 1000

int check_path(char *curr_dir, char *init_dir, char *str)
{
    char check[MAX];
    memset(check, '\0', MAX);
    strcpy(check, curr_dir);
    strcat(check, "/");
    char test[MAX];
    memset(test, '\0', MAX);
    if (strncmp(str, "./", 2) == 0)
    {
        int i;
        for (i = 2; i < strlen(str); i++)
        {
            test[i - 2] = str[i];
        }
        test[i - 2] = '\0';
    }
    else if (strncmp(str, "~", 1) == 0)
    {
        strcpy(test, init_dir);
        int i;
        char temp[MAX];
        memset(temp, '\0', MAX);
        for (i = 1; i < strlen(str); i++)
        {
            temp[i - 1] = str[i];
        }
        temp[i - 1] = '\0';
        strcat(test, temp);
        strcpy(check, "");
    }
    else
    {
        strcpy(test, str);
    }
    strcat(check, test);
    char *token3 = strtok(NULL, "/");
    while (token3 != NULL)
    {
        strcat(check, "/");
        strcat(check, token3);
        token3 = strtok(NULL, "/");
    }

    if (access(check, F_OK) == -1)
    {
        perror("No such path present");
        return 0;
    }
    return 1;
}