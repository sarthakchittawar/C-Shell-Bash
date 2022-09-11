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

#include "check_dir.h"
#include "check_path.h"
#include "give_path.h"
#include "sort.h"
#include "print_dir.h"

#define MAX 1000

int ls(char *dir, char *init_dir)
{
    struct dirent **array;

    int i = 0;

    if ((i = scandir(dir, &array, NULL, sort)) != -1)
    {

        for (int j = 0; j < i; j++)
        {
            if (strncmp(array[j]->d_name, ".", 1) != 0)
            {
                print_dir(array[j]);
            }
        }
        printf("\n");
    }
    else
    {
        perror("Current Directory is corrupt");
        free(array);
        return 0;
    }

    free(array);
    return 1;
}

int ls_a(char *dir, char *init_dir)
{
    struct dirent **array;

    int i = 0;

    if ((i = scandir(dir, &array, NULL, sort)) != -1)
    {

        for (int j = 0; j < i; j++)
        {
            print_dir(array[j]);
        }
        printf("\n");
    }
    else
    {
        perror("Current Directory is corrupt");
        free(array);
        return 0;
    }

    free(array);
    return 1;
}

int ls_l(char *dir, char *curr_dir, char *init_dir)
{
    struct dirent **array;

    int i = 0;

    if ((i = scandir(dir, &array, NULL, sort)) != -1)
    {
        struct stat st;
        char file[MAX];
        give_path(curr_dir, init_dir, dir, file);
        if (stat(file, &st) == 0)
        {
            int total = 0;
            for (int j = 0; j < i; j++)
            {
                if (strncmp(array[j]->d_name, ".", 1) != 0)
                {
                    char f[MAX];
                    char temp[MAX];
                    strcpy(temp, dir);
                    strcat(temp, "/");
                    strcat(temp, array[j]->d_name);
                    give_path(curr_dir, init_dir, temp, f);
                    struct stat s;
                    stat(f, &s);
                    total += s.st_blocks;
                }
            }
            printf("total %d\n", total / 2);
            for (int j = 0; j < i; j++)
            {
                if (strncmp(array[j]->d_name, ".", 1) != 0)
                {
                    char f[MAX];
                    char temp[MAX];
                    strcpy(temp, dir);
                    strcat(temp, "/");
                    strcat(temp, array[j]->d_name);
                    give_path(curr_dir, init_dir, temp, f);
                    struct stat s;
                    if (stat(f, &s) == 0)
                    {
                        DIR *directory = opendir(f);
                        if (directory == NULL)
                        {
                            printf("-");
                        }
                        else
                        {
                            printf("d");
                        }
                        if (s.st_mode & S_IRUSR)
                        {
                            printf("r");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IWUSR)
                        {
                            printf("w");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IXUSR)
                        {
                            printf("x");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IRGRP)
                        {
                            printf("r");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IWGRP)
                        {
                            printf("w");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IXGRP)
                        {
                            printf("x");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IROTH)
                        {
                            printf("r");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IWOTH)
                        {
                            printf("w");
                        }
                        else
                        {
                            printf("-");
                        }
                        if (s.st_mode & S_IXOTH)
                        {
                            printf("x");
                        }
                        else
                        {
                            printf("-");
                        }
                        printf(" ");
                        printf("%1ld ", s.st_nlink);
                        struct passwd *u = getpwuid(s.st_uid);
                        printf("%s ", u->pw_name);
                        struct passwd *g = getpwuid(s.st_gid);
                        printf("%s ", g->pw_name);
                        printf("%6ld ", s.st_size);

                        struct tm *time = localtime(&s.st_mtime);
                        char temp[MAX];
                        strftime(temp, MAX, "%b ", localtime(&s.st_mtime));
                        printf("%s", temp);
                        printf("%2d ", time->tm_mday);
                        strftime(temp, MAX, "%H:%M", localtime(&s.st_mtime));
                        printf("%2s ", temp);

                        print_dir(array[j]);
                        if (j != i - 1)
                        {
                            printf("\n");
                        }
                    }
                    else
                    {
                        perror("Stat error w/ filename");
                        return 0;
                    }
                }
            }
            printf("\n");
        }
        else
        {
            perror("Stat error w/ filename");
            return 0;
        }
    }
    else if (access(dir, F_OK) != -1)
    {
        char f[MAX];
        give_path(curr_dir, init_dir, dir, f);
        struct stat s;
        if (stat(f, &s) == 0)
        {
            printf("-");

            if (s.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            printf("%1ld ", s.st_nlink);
            struct passwd *u = getpwuid(s.st_uid);
            printf("%s ", u->pw_name);
            struct passwd *g = getpwuid(s.st_gid);
            printf("%s ", g->pw_name);
            printf("%6ld ", s.st_size);

            struct tm *time = localtime(&s.st_mtime);
            char temp[MAX];
            strftime(temp, MAX, "%b ", localtime(&s.st_mtime));
            printf("%s", temp);
            printf("%2d ", time->tm_mday);
            strftime(temp, MAX, "%H:%M", localtime(&s.st_mtime));
            printf("%2s ", temp);

            // dir = basename(dir);

            char pr[MAX];
            if (strncmp(dir, init_dir, strlen(init_dir)) == 0)
            {
                strcpy(pr, &dir[strlen(init_dir) + 1]);
            }
            else
            {
                strcpy(pr, dir);
            }
            strcpy(dir, pr);

            struct stat sb;
            if (stat(f, &sb) == 0 && sb.st_mode & S_IXUSR)
            {
                printf("\033[1;32m");
                printf("%s  ", dir);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s  ", dir);
            }
            printf("\033[0;37m");
            printf("\n");
        }
        else
        {
            perror("Stat error w/ filename");
            return 0;
        }
    }
    else
    {
        perror("Current Directory is corrupt");
        free(array);
        return 0;
    }

    free(array);
    return 1;
}

int ls_al(char *dir, char *curr_dir, char *init_dir)
{
    struct dirent **array;

    int i = 0;

    if ((i = scandir(dir, &array, NULL, sort)) != -1)
    {
        struct stat st;
        char file[MAX];
        give_path(curr_dir, init_dir, dir, file);
        if (stat(file, &st) == 0)
        {
            int total = 0;
            for (int j = 0; j < i; j++)
            {

                char f[MAX];
                char temp[MAX];
                strcpy(temp, dir);
                strcat(temp, "/");
                strcat(temp, array[j]->d_name);
                give_path(curr_dir, init_dir, temp, f);
                struct stat s;
                stat(f, &s);
                total += s.st_blocks;
            }
            printf("total %d\n", total / 2);
            for (int j = 0; j < i; j++)
            {

                char f[MAX];
                char temp[MAX];
                strcpy(temp, dir);
                strcat(temp, "/");
                strcat(temp, array[j]->d_name);
                give_path(curr_dir, init_dir, temp, f);
                struct stat s;
                if (stat(f, &s) == 0)
                {
                    DIR *directory = opendir(f);
                    if (directory == NULL)
                    {
                        printf("-");
                    }
                    else
                    {
                        printf("d");
                    }
                    if (s.st_mode & S_IRUSR)
                    {
                        printf("r");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IWUSR)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IXUSR)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IRGRP)
                    {
                        printf("r");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IWGRP)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IXGRP)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IROTH)
                    {
                        printf("r");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IWOTH)
                    {
                        printf("w");
                    }
                    else
                    {
                        printf("-");
                    }
                    if (s.st_mode & S_IXOTH)
                    {
                        printf("x");
                    }
                    else
                    {
                        printf("-");
                    }
                    printf(" ");
                    printf("%1ld ", s.st_nlink);
                    struct passwd *u = getpwuid(s.st_uid);
                    printf("%s ", u->pw_name);
                    struct passwd *g = getpwuid(s.st_gid);
                    printf("%s ", g->pw_name);
                    printf("%6ld ", s.st_size);

                    struct tm *time = localtime(&s.st_mtime);
                    char temp[MAX];
                    strftime(temp, MAX, "%b ", localtime(&s.st_mtime));
                    printf("%s", temp);
                    printf("%2d ", time->tm_mday);
                    strftime(temp, MAX, "%H:%M", localtime(&s.st_mtime));
                    printf("%2s ", temp);

                    print_dir(array[j]);
                    if (j != i - 1)
                    {
                        printf("\n");
                    }
                }
                else
                {
                    perror("Stat error w/ filename");
                    return 0;
                }
            }
            printf("\n");
        }
        else
        {
            perror("Stat error w/ filename");
            return 0;
        }
    }
    else if (access(dir, F_OK) != -1)
    {
        char f[MAX];
        give_path(curr_dir, init_dir, dir, f);
        struct stat s;
        if (stat(f, &s) == 0)
        {
            printf("-");

            if (s.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (s.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            printf("%1ld ", s.st_nlink);
            struct passwd *u = getpwuid(s.st_uid);
            printf("%s ", u->pw_name);
            struct passwd *g = getpwuid(s.st_gid);
            printf("%s ", g->pw_name);
            printf("%6ld ", s.st_size);

            struct tm *time = localtime(&s.st_mtime);
            char temp[MAX];
            strftime(temp, MAX, "%b ", localtime(&s.st_mtime));
            printf("%s", temp);
            printf("%2d ", time->tm_mday);
            strftime(temp, MAX, "%H:%M", localtime(&s.st_mtime));
            printf("%2s ", temp);

            // dir = basename(dir);

            char pr[MAX];
            if (strncmp(dir, init_dir, strlen(init_dir)) == 0)
            {
                strcpy(pr, &dir[strlen(init_dir) + 1]);
            }
            else
            {
                strcpy(pr, dir);
            }
            strcpy(dir, pr);

            struct stat sb;
            if (stat(f, &sb) == 0 && sb.st_mode & S_IXUSR)
            {
                printf("\033[1;32m");
                printf("%s  ", dir);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s  ", dir);
            }
            printf("\033[0;37m");
            printf("\n");
        }
        else
        {
            perror("Stat error w/ filename");
            return 0;
        }
    }
    else
    {
        perror("Current Directory is corrupt");
        free(array);
        return 0;
    }
    return 1;
}

int ls_main(char *process_name, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, char **arr, int c)
{
    if (c == 1)
    {
        if (!ls(".", init_dir))
        {
            return 0;
        }
    }
    else if (c == 2 && strcmp(arr[1], "-a") == 0)
    {
        if (!ls_a(".", init_dir))
        {
            return 0;
        }
    }
    else if (c == 2 && strcmp(arr[1], "-l") == 0)
    {
        if (!ls_l(".", curr_dir, init_dir))
        {
            return 0;
        }
    }
    else if ((c == 3 && ((strcmp(arr[1], "-a") == 0 && strcmp(arr[2], "-l") == 0) || (strcmp(arr[1], "-l") == 0 && strcmp(arr[2], "-a") == 0)) || (c == 2 && (strcmp(arr[1], "-al") == 0 || strcmp(arr[1], "-la") == 0))))
    {
        if (!ls_al(".", curr_dir, init_dir))
        {
            return 0;
        }
    }
    else
    {
        int a = 0;
        int l = 0;
        int dirno = 0;
        char **dirr = (char **)malloc(MAX * sizeof(char *));

        for (int i = 1; i < c; i++)
        {
            if (strncmp(arr[i], "-", 1) == 0)
            {
                for (int j = 1; j < strlen(arr[i]); j++)
                {
                    if (arr[i][j] == 'a')
                    {
                        a++;
                    }
                    else if (arr[i][j] == 'l')
                    {
                        l++;
                    }
                    else
                    {
                        perror("Wrong tag for ls");
                        return 0;
                    }
                }
            }
            else
            {
                dirr[dirno] = arr[i];
                dirno++;
            }
        }
        if (dirno == 0)
        {
            if (a == 0 && l == 0)
            {
                if (!ls(".", init_dir))
                {
                    return 0;
                }
            }
            else if (a > 0 && l == 0)
            {
                if (!ls_a(".", init_dir))
                {
                    return 0;
                }
            }
            else if (a == 0 && l > 0)
            {
                if (!ls_l(".", curr_dir, init_dir))
                {
                    return 0;
                }
            }
            else if (a > 0 && l > 0)
            {
                if (!ls_al(".", curr_dir, init_dir))
                {
                    return 0;
                }
            }
        }
        for (int i = 0; i < dirno; i++)
        {

            char file[MAX];
            memset(file, '\0', MAX);
            strcpy(file, dirr[i]);

            if (!check_dir(curr_dir, init_dir, file) && !check_path(curr_dir, init_dir, file))
            {
                return 0;
            }
            char check[MAX];
            strcpy(file, give_path(curr_dir, init_dir, file, check));
            DIR *dirp = opendir(file);
            if (dirp == NULL)
            {
                if (l == 0)
                {
                    char pr[MAX];
                    if (strncmp(file, init_dir, strlen(init_dir)) == 0)
                    {
                        strcpy(pr, &file[strlen(init_dir) + 1]);
                    }
                    else
                    {
                        strcpy(pr, file);
                    }

                    struct stat sb;
                    if (stat(file, &sb) == 0 && sb.st_mode & S_IXUSR)
                    {
                        printf("\033[1;32m");
                        printf("%s  ", pr);
                    }
                    else
                    {
                        printf("\033[0;37m");
                        printf("%s  ", pr);
                    }
                    printf("\n\033[0;37m");
                }
                else if (a == 0)
                {
                    ls_l(file, curr_dir, init_dir);
                }
                else
                {
                    ls_al(file, curr_dir, init_dir);
                }
            }
            else
            {
                struct dirent *dent = readdir(dirp);
                if (dent != NULL)
                {

                    if (dirno > 1)
                    {
                        printf("%s:\n", dirr[i]); // or file
                    }
                    if (a == 0 && l == 0)
                    {
                        if (!ls(file, init_dir))
                        {
                            return 0;
                        }
                    }
                    else if (a > 0 && l == 0)
                    {
                        if (!ls_a(file, init_dir))
                        {
                            return 0;
                        }
                    }
                    else if (a == 0 && l > 0)
                    {
                        if (!ls_l(file, curr_dir, init_dir))
                        {
                            return 0;
                        }
                    }
                    else if (a > 0 && l > 0)
                    {
                        if (!ls_al(file, curr_dir, init_dir))
                        {
                            return 0;
                        }
                    }

                    if (i != dirno - 1)
                    {
                        printf("\n");
                    }
                }
                else
                {
                    perror("Directory/File unrecognised");
                    return 0;
                }
            }
        }
        free(dirr);
    }
}