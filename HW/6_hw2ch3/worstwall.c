/************************************************
 *               worstwall.c                    *
 * Uses shared memory to replicate the ... 2nd  *
 * worst facebook wall ever                     *
 ************************************************
 * Author: Justin Weigle                        *
 * Edited: 20 Feb 2020                          *
 ************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    char choice;
    char messg[128];
    int status;

    int leng = 128;
    int protec = PROT_READ | PROT_WRITE;
    int flgs = MAP_SHARED | MAP_ANONYMOUS;
    void *shrmem = mmap(NULL, leng, protec, flgs, -1, 0);

    while (1) {
        printf("Would you like to set your status? y/n?\n");
        choice = getchar();
        fflush(stdin);

        if (choice == 'y') {
            printf("You chose yes\nNew status?:\n");
            scanf("%s", &messg);
            fflush(stdin);
            memcpy(shrmem, messg, sizeof(messg));
            messg[0] = '\0';
            choice = '\0';
        } else {
            printf("You chose no\n");
            exit(0);
        }
    }


    return 0;
}
