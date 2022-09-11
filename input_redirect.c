#include <stdio.h>
#include <unistd.h>

#include "give_path.h"

#define MAX 1000

int input_redirect(char *curr_dir, char *init_dir, FILE *fp, char *buf, char *filename)
{
    int stdin = fileno(fp);
    int old_stdin = dup(STDIN_FILENO);

    char filepath[MAX];
    give_path(curr_dir, init_dir, filename, filepath);
    if (FILE *input = fopen(filepath, "r") != NULL)
    {
        int in = fileno(input);
        if (dup2(in, STDIN_FILENO) != -1)
        {
            dup2(old_stdin, STDIN_FILENO);
        }
        close(in);
    }
    else
    {
        perror("No such input file present");
        close(old_stdin);
        return 0;
    }
    close(old_stdin);
    return 1;
}