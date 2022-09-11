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

int print_dir(struct dirent *array)
{
    struct stat sb;
    if (array->d_type == DT_DIR)
    {
        printf("\033[1;34m");
        printf("%s  ", array->d_name);
    }
    else if (stat(array->d_name, &sb) == 0 && sb.st_mode & S_IXUSR && array->d_type == DT_REG)
    {
        printf("\033[1;32m");
        printf("%s  ", array->d_name);
    }
    else
    {
        printf("\033[0;37m");
        printf("%s  ", array->d_name);
    }
    printf("\033[0;37m");
}