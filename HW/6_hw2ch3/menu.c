#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void run_selection (int choice)
{
    switch (choice) {
        case 0:
            exit(0);
            break;
        case 1:
            system("xdg-open https://www.youtube.com/watch?v=dQw4w9WgXcQ");
            break;
        default:
            break;
    }
}

int main()
{
    int userin;
    while (1) {
        printf("Super duper menu v2.0 - Enter a command #:\n");
        printf("0 - quit\n");
        printf("1 - play a cool vid\n");
        scanf("%d", &userin);
        run_selection(userin);
        system("clear");
        printf("We go agane\n");
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
