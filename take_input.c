#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#include "autocomplete.h"

#define MAX 1000

struct termios orig_termios;
extern int bg_processes[100000];
extern int bgcount;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
        die("tcsetattr");
    }
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        die("tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}

int take_input(char text[MAX], char *prompt, char *dir)
{
    char c;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        int i = 0;

        while (scanf("%c", &c) && c != '\n' && c != '\r')
        {
            if (iscntrl(c))
            {
                if (c == 9)
                {
                    // autocomplete func with text so far
                    autocomplete(text, &i, prompt, dir);
                }
                else if (c == 127)
                {
                    if (i > 0)
                    {
                        i--;
                        text[i] = '\0';
                        printf("\b \b"); // moves cursor, adds space, and moves it back again
                    }
                }
                else if (c == 4) // ctrl-d
                {
                    for (int j = 0; j < bgcount; j++)
                    {
                        if (bg_processes[j] != -1)
                        {
                            kill(bg_processes[j], SIGINT);
                        }
                    }
                    exit(0);
                }
            }
            else
            {
                printf("%c", c);
                text[i] = c;
                i++;
            }
        }
        text[i] = '\n';
        disableRawMode();
        if (c == '\n' || c == '\r')
        {
            break;
        }
    }
    printf("\n");

    return 1;
}