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

int trim(char *s)
{
    char *x = (char *)malloc(MAX);
    int c = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
        {
            break;
        }
        c++;
    }
    int d = strlen(s) - 1;
    for (int i = d; i >= c; i--)
    {
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
        {
            break;
        }
        d--;
    }
    for (int i = c; i <= d; i++)
    {
        x[i - c] = s[i];
    }
    x[d - c + 1] = '\0';
    strcpy(s, x);
    free(x);
    return 1;
}