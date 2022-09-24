#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define MAX 1000

int autocomplete(char text[], int *length, char *prompt, char *dir)
{
    char curr_dir[MAX];
    getcwd(curr_dir, MAX);
    char str[MAX];
    strcpy(str, text);
    char *token2 = strtok(str, " \t");
    char token[MAX];
    while (token2 != NULL)
    {
        strcpy(token, token2);
        token2 = strtok(NULL, " \t");
    }
    // now, token has last word of input
    struct dirent **array;
    int num;
    int count = 0;
    if ((num = scandir(curr_dir, &array, NULL, alphasort)) != -1)
    {
        struct dirent **autofill = (struct dirent **)malloc(MAX * sizeof(struct dirent **));
        for (int i = 0; i < num; i++)
        {
            if (strncmp(token, array[i]->d_name, strlen(token)) == 0)
            {
                autofill[count] = array[i];

                count++;
            }
        }
        if (count == 1)
        {
            strcat(text, &(autofill[0]->d_name)[strlen(token)]);
            printf("%s", &(autofill[0]->d_name)[strlen(token)]);
            if (autofill[0]->d_type == DT_DIR)
            {
                strcat(text, "/");
                printf("/");
            }
            else if (autofill[0]->d_type == DT_REG)
            {
                strcat(text, " ");
                printf(" ");
            }
            *length = strlen(text);
        }
        else if (count > 1)
        {
            printf("\n");
            for (int i = 0; i < count; i++)
            {
                printf("%s", autofill[i]->d_name);
                if (autofill[i]->d_type == DT_DIR)
                {
                    printf("/");
                }
                else if (autofill[i]->d_type == DT_REG)
                {
                    printf(" ");
                }
                printf("\n");
            }
            // prompt again
            printf("\033[1;32m");
            printf("%s", prompt);
            printf("\033[1;37m");
            printf(":");
            printf("\033[1;34m");
            printf("%s> ", dir);
            printf("\033[0;37m");
            printf("%s", text);
        }
    }
}