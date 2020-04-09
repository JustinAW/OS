#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#ifndef INPUT_BUFF_SIZE
#define INPUT_BUFF_SIZE 2048
#endif
#ifndef TWEET_BUFF_SIZE
#define TWEET_BUFF_SIZE 146
#endif

void* friennnd (void *tweet_buff) {
    static int i = 0;
    i++;
    printf("I'm friend number %d\n", i);
}

int main (int argc, char **argv)
{
    char input_buff[INPUT_BUFF_SIZE];
    char tweet_buff[TWEET_BUFF_SIZE];
    int tweet_num = 0;

    printf("How many friends should tweet, Adam? ");
    int tn_stat = scanf("%d", &tweet_num); // get tweet_num
    /* Check for scanf errors */
    if (tn_stat == EOF) {
        if (!errno) {
            printf("EOF reached\n");
        } else {
            perror("EOF reached");
            exit(-1);
        }
    }
    if (tn_stat == 0) {
        while (fgetc(stdin) != '\n'); // Read until a newline is found
        printf("No valid number of tweets found\n");
        exit(-1);
    }
    if (tweet_num <= 0) {
        printf("No tweets this time, huh?\nExiting...\nGoobye Adam!\n");
        exit(0);
    }

    /* init all pthread variables */
    pthread_attr_t pthr_attr;
    pthread_attr_init(&pthr_attr);
    pthread_t tids[tweet_num];

    /* create threads */
    for (int i = 0; i < tweet_num; i++) {
        if (pthread_create(&tids[i], &pthr_attr, friennnd, tweet_buff)) {
            perror("Failed to create thread");
            pthread_attr_destroy(&pthr_attr);
            exit(-1);
        }
    }
    /* wait on each thread */
    for (int i = 0; i < tweet_num; i++) {
        pthread_join(tids[i], NULL);
    }

    pthread_attr_destroy(&pthr_attr);
    return 0;
}
