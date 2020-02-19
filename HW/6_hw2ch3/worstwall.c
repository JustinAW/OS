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
#include <sys/mman.h>

int main(int argc, char **argv)
{
    char userin;
    char *messg;

    int leng = 256;
    int protec = PROT_READ | PROT_WRITE;
    int flgs = MAP_SHARED | MAP_ANONYMOUS;
    void *shrmem = mmap(NULL, leng, protec, flgs, -1, 0);

    printf("Status:\n");
    printf("%s\n", shrmem);

    printf("Would you like to change your status? y/n?\n");
    scanf("%c", &userin);

    if (userin == 'y') {
        printf("You chose yes\nNew status?:\n");
        scanf("%s", &shrmem);
    } else {
        printf("You chose no\n");
    }

    return 0;
}
