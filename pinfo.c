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

int pinfo(int c, char **arr, char *init_dir, int bg_processes[], int bgcount)
{
    pid_t pid;
    if (c == 1)
    {
        pid = getpid();
    }
    else if (c == 2)
    {
        pid = atoi(arr[1]);

        if (getpgid(pid) < 0 || pid == 0)
        {
            perror("Invalid PID entered");
            return 0;
        }
    }
    else
    {
        perror("Invalid syntax for pinfo");
        return 0;
    }
    char file[MAX] = "/proc/";
    char buffer[MAX];
    sprintf(buffer, "%d", pid);
    strcat(file, buffer);
    strcat(file, "/stat");
    FILE *proc = fopen(file, "r");
    char status;
    long unsigned int memory;
    int fg;
    if (fscanf(proc, "%d %*s %c %*d %*d %*d %*d %d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %lu", &pid, &status, &fg, &memory))
    {
        if (status != 'R' && status != 'S' && status != 'Z')
        {
            perror("PID out of scope");
            return 0;
        }
        printf("pid : %d\n", pid);
        printf("process status : %c", status);
        int flag = 0;
        for (int i = 0; i < bgcount; i++)
        {
            if (bg_processes[i] == pid)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0 && status != 'Z')
        {
            printf("+");
        }
        printf("\nmemory : %lu {Virtual Memory}\n", memory);
    }
    else
    {
        perror("Proc or scan error");
        return 0;
    }
    // path
    char buf[PATH_MAX], exec[PATH_MAX];
    sprintf(buf, "/proc/%d/exe", pid);
    if (readlink(buf, exec, PATH_MAX) != -1)
    {

        if (strncmp(exec, init_dir, strlen(init_dir)) == 0)
        {
            printf("executable path : ~%s\n", &exec[strlen(init_dir)]);
        }
        else
        {
            printf("executable path : ~%s\n", exec);
        }
    }
    else
    {
        perror("Error in executable path reading");
        return 0;
    }

    return 1;
}