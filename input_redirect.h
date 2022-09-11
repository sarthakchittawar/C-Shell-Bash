#include <stdio.h>

#ifndef INPUT_REDIRECT
#define INPUT_REDIRECT

int input_redirect(char *curr_dir, char *init_dir, FILE *fp, char *buf, char *filename);

#endif