#ifndef PROCESS
#define PROCESS

int process(char *process_name, int andflag, char *dir, char *curr_dir, char *init_dir, char *prev_dir, char *username, int *timeflag, char *prompt, char **hist, int histno, int *bgcount, int bg_processes[], char **bg_procname, int bg_bitmaps[], double *bef, double *en);
#endif