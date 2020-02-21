#include <stdio.h>
#include <unistd.h>

int is_prime (int n)
{
    for (int i = n-1; i > 1; i--) {
        if (i % 2 == 0) {
            return 0;
        }
        i--;
    }
    return 1;
}

int main(int argc, char **argv)
{
    int pp[2];

    if (pipe(pp) < 1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid > 0) { // parent process
        /***** WRITE *****/
        close(pp[0]); // close read side of pipe
        for (int i = 0; i < 51; i++) {
            write(pp[1], &i, 1);
        }
        int i = -1;
        write(pp[1], &i, 1);
        close(pp[1]); // close write side of pipe
    } else { // child process
        /***** READ *****/
        int read_num;
        close(pp[1]); // close write side of pipe
        read(pp[0], &read_num, 1);
        close(pp[0]); // close read side of pipe

        if (is_prime(read_num)) {
            printf("Prime: %d\n");
        }
    }


    return 0;
}
