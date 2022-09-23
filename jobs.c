#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

extern int bg_processes[100000];
extern char **bg_procname;

int cmpfnc(const void *a, const void *b)
{
    int *c = (int *)a;
    int *d = (int *)b;
    return (strcmp(bg_procname[*c], bg_procname[*d]));
}

int jobs(int bgcount, char **bg_procname, int bg_bitmaps[], char **arr, int c)
{
    if (strcmp(arr[0], "jobs") == 0)
    {
        int r = 0, s = 0;
        for (int i = 1; i < c; i++)
        {
            if (strncmp(arr[i], "-", 1) == 0)
            {
                for (int j = 1; j < strlen(arr[i]); j++)
                {
                    if (arr[i][j] == 'r')
                    {
                        r++;
                    }
                    else if (arr[i][j] == 's')
                    {
                        s++;
                    }
                    else
                    {
                        perror("Invalid Flag");
                        return 0;
                    }
                }
            }
        }
        int sorted[bgcount];
        int c = 0;
        for (int i = 0; i < bgcount; i++)
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
                    if (status == 'T')
                    {
                        bg_bitmaps[i] = 0;
                    }
                    else
                    {
                        bg_bitmaps[i] = 1;
                    }
                }
                fclose(proc);
            }
            else
            {
                bg_processes[i] = -1;
            }

            if (bg_processes[i] > 0)
            {
                sorted[c] = i;
                c++;
            }
        }
        qsort(sorted, c, sizeof(int), cmpfnc);
        for (int i = 0; i < c; i++)
        {
            if (bg_bitmaps[sorted[i]] == 1 && ((r == 0 && s == 0) || (r != 0)))
            {
                printf("[%d] Running %s [%d]\n", sorted[i] + 1, bg_procname[sorted[i]], bg_processes[sorted[i]]);
            }
            else if (bg_bitmaps[sorted[i]] == 0 && ((r == 0 && s == 0) || (s != 0)))
            {
                printf("[%d] Stopped %s [%d]\n", sorted[i] + 1, bg_procname[sorted[i]], bg_processes[sorted[i]]);
            }
        }
    }
    return 1;
}