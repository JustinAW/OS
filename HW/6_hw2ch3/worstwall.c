/************************************************
 *               worstwall.c                    *
 * Uses shared memory to replicate the ... 2nd  *
 * worst facebook wall ever                     *
 ************************************************
 * Author: Justin Weigle                        *
 * Edited: 19 Feb 2020                          *
 ************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    char userin;
    char *messg = "";
    int status;

    int leng = 256;
    int protec = PROT_READ | PROT_WRITE;
    int flgs = MAP_SHARED | MAP_ANONYMOUS;
    void *shrmem = mmap(NULL, leng, protec, flgs, -1, 0);

    memcpy(shrmem, messg, sizeof(messg));

    while (1) {
        printf("Status:\n");
        printf("%s\n", shrmem);

        int pid = fork();

        if (pid < 0) { // error if fork returns < 0
            fprintf(stderr, "Fork Failed\n");
            exit(-1);
        } else if (pid == 0) { // child
            printf("Would you like to change your status? y/n?\n");
            scanf("%c", &userin);
            fflush(stdin);

            if (userin == 'y') {
                printf("You chose yes\nNew status?:\n");
                //scanf("%s", &messg);
                fgets(messg, sizeof(shrmem), stdin);
                memcpy(shrmem, messg, sizeof(messg));
            } else {
                printf("You chose no\n");
            }

            exit(0);
        } else { // parent
            while(1) {
                pid_t pid1 = waitpid(-1, &status, 0);
                if (pid1 <= 0) break;          // ^ wait around
            }
        }
    }


    return 0;
}
