#include <stdio.h>

#define MAX 1000

int take_input(char text[MAX])
{
    char c;
    scanf("%c", &c);
    int i = 0;
    while (c != '\n' && c != '\r')
    {
        text[i] = c;
        i++;
        scanf("%c", &c);
    }
    text[i] = '\n';
}