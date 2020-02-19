/************************************************
 *                  menu.c                      *
 * Displays a menu with 5 options to execute.   *
 * Uses fork() and exec() to run the program    *
 * as a separate process and wait on it to      *
 * finish before returning to the menu.         *
 ************************************************
 * Author: Justin Weigle                        *
 * Edited: 19 Feb 2020                          *
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/************************************************
 *              run_selection                   *
 ************************************************
 * Executes the command chosen by the user      *
 * @param unsigned int choice                   *
 ************************************************/
void run_selection (unsigned int choice)
{
    switch (choice) {
        case 1:
            execlp("xdg-open", "xdg-open",
                    "https://www.youtube.com/watch?v=dQw4w9WgXcQ", NULL);
            break;
        case 2:
            execlp("cowsay", "cowsay", "hi", NULL);
            break;
        case 3:
            execlp("lsblk", "lsblk", NULL);
            break;
        case 4:
            execlp("cmatrix", "cmatrix", NULL);
            break;
        case 5:
            execlp("clear", "clear", NULL);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    unsigned int userin;
    int status;

    /***** Loop until user chooses to exit with 0 *****/
    while (1) {
        /************** The Menu ****************/
        printf("Super duper menu v2.0 - Enter a command #:\n");
        printf("0 - quit\n");
        printf("1 - play a cool vid\n");
        printf("2 - have a cow say hi to you\n");
        printf("3 - list attached disk drives\n");
        printf("4 - look like a hacker boi\n");
        printf("5 - clear the screen, it's getting cluttered\n");
        fflush(stdout);

        /***** get user's choice *****/
        scanf("%u", &userin);
        if (userin == 0) { // user chooses 0 exit immediately
            exit(0);
        }

        /***** fork here *****/
        pid_t pid = fork();
        if (pid < 0) { // error if fork returns < 0
            fprintf(stderr, "Fork Failed\n");
            exit(-1);
        } else if (pid == 0) { // child
            run_selection(userin);
            exit(0);
        } else { // parent
            while(1) {
                pid_t pid1 = waitpid(-1, &status, 0);
                if (pid1 <= 0) break;          // ^ wait around
            }
        }
        printf("\n--------------------------------------------\n");
    }
    return 0;
}
