# C-Shell-Bash
Implementation of the various features in terminal (bash) using C

## How to run project?
Run `make all`, then `./a.out` on the folder where file is downloaded

## File description - 
1. **`driver.c`** - Contains all global variables, initialisations and input parsing which handles both ';' and '&' as separators using strtok
2. **`trim.c`** - Includes a function which takes a string and removes all leading and ending subspaces
3. **`process.c`** - The input array is put into an array and each substring is trimmed. Depending on the first parameter, various functions are executed
4. **`take_input.c`** - Takes input into a string variable character by character (includes spaces)
5. **`add_hist.c`** - Function which adds the input into history if some conditions are followed, as given in pdf.
6. **`cd.c`** - Contains the implementation of the 'cd' function in the shell
7. **`check_dir.c`** - Checks if path exists in system as a directory
8. **`check_path.c`** - Checks if path exists in system as a file/directory
9. **`discover.c`** - Contains the implementation of the discover functionality using recursive search
10. **`give_path.c`** - In this function, a string such as "a.txt", "./a.txt", "~/a.txt", etc is given and it outputs the path of this file (if exists) w.r.t. the current directory.
11. **`handler.c`** - The signal() function returns to this function if a child process changes state(i.e. interrupt detected). This function stores the post-completion text in a buffer, which is displayed onto terminal after the next shell prompt (Just like normal bash)
12. **`history.c`** - Displays latest 10 commands from history array
13. **`ls.c`** - Contains the detailed implementation of the 'ls' command with akk optional parameters and display specifications
14. **`others.c`** - Contains the implementation of the foreground/background processes(child) using fork(), execvp() & wait() functions
15. **`pinfo.c`** - Contains the implementation of the pinfo command which accesses /proc/<pid>/stat and /proc/<pid>/exe files to scan information about the process
16. **`print_dir.c`** - Takes in a 'struct dirent *' variable and prints it in some colour (Blue/Green/White) depending on its file type
17. **`sort.c`** - Function to sort in ls

## Side Notes -
1. *History is dynamic across all sessions as it is stored in a hidden file `.history` after each session. Now, it is accessed when shell starts and stored in an array of max size 20 where all changes happen.*
2. *Max no. of parameters/arguments is 1000, else sigsegv*
3. *In ls and ls -a, decreasing width of shell window will disturb the neatness of the output, so full window size is preferable to compare with actual shell*