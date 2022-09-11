#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

int add_hist(int *histno, char *text, char **hist)
{
    (*histno)++;
    if (strcmp(text, "") == 0 || strcmp(text, "\n") == 0 || strcmp(text, "\t") == 0)
    {
        memset(text, 0, sizeof(text));
        return 0;
    }

    if (*histno > 21)
    {
        perror("History corrupted. Please delete & recreate your hidden file .history");
        return 0;
    }
    if (*histno > 20)
    {
        if (strcmp(text, hist[(*histno) - 2]) != 0)
        {
            for (int j = 1; j < 20; j++)
            {
                strcpy(hist[j - 1], hist[j]);
            }
            strcpy(hist[19], text);
        }

        (*histno)--;
    }
    else
    {
        if ((*histno) == 1)
        {
            strcpy(hist[0], text);
        }
        else if (strcmp(text, hist[(*histno) - 2]) != 0)
        {
            strcpy(hist[(*histno) - 1], text);
        }
        else
        {
            (*histno)--;
        }
    }
    return 1;
}