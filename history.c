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

#define MAX 1000

int history(int c, char **hist, int histno)
{
    int j;
    if (histno >= 10)
    {
        j = 10;
    }
    else
    {
        j = histno;
    }
    for (int i = histno - j; i <= histno - 1; i++)
    {
        printf("%s", hist[i]);
    }
    return 1;
}