#ifndef LS_H
#define LS_H

int ls_main(char *process_name, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, char **arr, int c);
int ls(char *dir, char *init_dir);
int ls_a(char *dir, char *init_dir);
int ls_l(char *dir, char *curr_dir, char *init_dir);
int ls_al(char *dir, char *curr_dir, char *init_dir);

#endif