#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void run_selection (int choice)
{
    switch (choice) {
        case 1:
            system("xdg-open https://www.youtube.com/watch?v=dQw4w9WgXcQ");
            break;
        case 2:
            system("cowsay hi");
            break;
        case 3:
            system("ssh jw6858@sloop.engr.ship.edu");
            break;
        case 4:
            system("lsblk");
            break;
        case 5:
            system("cmatrix");
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    unsigned int userin;
    int status;

    while (1) {
        printf("Super duper menu v2.0 - Enter a command #:\n");
        printf("0 - quit\n");
        printf("1 - play a cool vid\n");
        printf("2 - have a cow say hi\n");
        printf("3 - ssh into sloop\n");
        printf("4 - list attached disk drives\n");
        printf("5 - look like a hacker boi\n");
        fflush(stdout);
        scanf("%u", &userin);
        if (userin == 0) {
            exit(0);
        }

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork Failed\n");
            exit(-1);
        } else if (pid == 0) {
            run_selection(userin);
            exit(0);
        }
        else {
            while(1) {
                pid_t pid1 = waitpid(-1, &status, WNOHANG);
                if (pid1 <= 0) break;
            }
        }
        printf("\n--------------------------------------------\n\n");
    }
//    pid_t pid, pid1;
//
//    /* fork a child process */
//    pid = fork();
//
//    if (pid < 0) {
//        fprintf(stderr, "Fork Failed\n");
//    } else if (pid == 0) {
//        pid1 = getpid();
//        printf("child: pid = %d\n", pid);
//        printf("child: pid1 = %d\n", pid1);
//    } else {
//        pid1 = getpid();
//        printf("parent: pid = %d\n", pid);
//        printf("parent: pid1 = %d\n", pid1);
//        wait(NULL);
//    }

    return 0;
}
