/************************************************
 *               worstwall.c                    *
 * Uses shared memory to replicate the 2nd      *
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

#define PAGESIZE 4096

int main(int argc, char **argv)
{
    char choice;
    char userin[4096];

    /***** allocate the shared memory *****/
    char *shrmem = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    while (1) {
        printf("---------------\nCurrent status:\n---------------\n");
        /***** print the contents of the shared memory *****/
        printf("%s", shrmem);
        printf("---------------\n");
        printf("Would you like to change your status? y/n?\n");
        choice = getchar();
        getchar(); // eat newline, nom nom

        if (choice == 'y') {
            printf("You chose yes\nNew status?:\n");
            fgets(userin, sizeof(userin), stdin);
            /***** store the users input in the shared memory *****/
            memcpy(shrmem, userin, sizeof(userin));
        } else if (choice == 'n') {
            printf("You chose no\n");
            exit(0);
        } else {
            printf("Not a valid choice, press Enter to continue\n");
            while (getchar() != '\n') {}
        }
    }


    return 0;
}
