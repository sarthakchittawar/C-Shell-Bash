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
#include <ctype.h>

#include "sort.h"

#define MAX 1000

int sort(const struct dirent **a, const struct dirent **b)
{
    return sortfunc((char *)((*a)->d_name), (char *)((*b)->d_name));
}

int sortfunc(char *p, char *q)
{
    char a[MAX], b[MAX];
    strcpy(a, p);
    strcpy(b, q);
    for (int i = 0; i < strlen(a); i++)
    {
        a[i] = tolower(a[i]);
    }
    for (int i = 0; i < strlen(b); i++)
    {
        b[i] = tolower(b[i]);
    }
    char c[MAX], d[MAX];
    if (a[0] == '.' && strlen(a) > 1)
    {
        strcpy(c, &a[1]);
        if (strcmp(a, "..") == 0)
        {
            strcpy(c, a);
        }
    }
    else
    {
        strcpy(c, a);
    }
    if (b[0] == '.' && strlen(b) > 1)
    {
        strcpy(d, &b[1]);
        if (strcmp(b, "..") == 0)
        {
            strcpy(d, b);
        }
    }
    else
    {
        strcpy(d, b);
    }
    return (strcmp(c, d));
}