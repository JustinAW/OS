/************************************************
 *              Adam's Friends                  *
 *               Hw Chapter 7                   *
 ************************************************
 * Asks Adam for the number of friends he wants *
 * to retweet his tweets, then asks for those   *
 * friends' names, splits the friends off into  *
 * their own threads, and those threads read    *
 * from the buffer that Adam is writing to.     *
 * Uses semaphores to maintain mutual exclusion *
 ************************************************
 * Author: Justin Weigle                        *
 * Edited: 11 Apr 2020                          *
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#ifndef INPUT_BUFF_SIZE
#define INPUT_BUFF_SIZE 2048 // for putting in friends' names
#endif
#ifndef TWEET_BUFF_SIZE
#define TWEET_BUFF_SIZE 146 // for Adam's tweets
#endif

/* for tokenizing input friends' names */
typedef enum {
    Init_State,
    Letter_State,
    Blank_State
} Name_Read_State;

/* For input --> linked list */
typedef struct Friend_Node {
    char *name;
    struct Friend_Node *next;
} Friend_Node;

/* For linked list management */
struct Friend_List {
    Friend_Node *head;
    Friend_Node *tail;
    int count;
};

/* For Adam and friennnds */
struct Tweeter {
    char *name;
    void *tweet;
    void *rw_sem;
    void *r_sem;
    int *read_count;
    int *diff;
};

static bool split_friends (struct Friend_List *, char *);
static void save_friend (char *, struct Friend_List **);
static void free_friend_list (struct Friend_List *);

/**
 * The reader thread "Adam's friends"
 */
void* friennnd (void *tweeter) {
    struct Tweeter *friend = (struct Tweeter*)tweeter;
    char *tweet = (char *)friend->tweet;
    char *name = (char *)friend->name;
    sem_t *rw_sem = (sem_t *)friend->rw_sem;
    sem_t *r_sem = (sem_t *)friend->r_sem;
    int *read_count = friend->read_count;
    int *diff = friend->diff;

    int prev_diff = *diff;

    while (true) {
        sem_wait(r_sem); // lock reading
            (*read_count)++;
            if (*read_count == 1) { // if first reader
                sem_wait(rw_sem); // lock writing
            }
        sem_post(r_sem); // unlock reading

        if (*diff > prev_diff) { // if it's a new message
            prev_diff = *diff; // remember previous msg num
            printf("%s tweeted: %s\n", name, tweet);
        }

        sem_wait(r_sem); // lock reading
            (*read_count)--;
            if (*read_count == 0) { // if last reader
                sem_post(rw_sem); // unlock writing
            }
        sem_post(r_sem); // unlock reading
    }
}

/**
 * The writer thread "Adam"
 */
void* Adam (void *tweeter)
{
    struct Tweeter *adam = (struct Tweeter*)tweeter;
    char *tweet_buff = (char *)adam->tweet;
    sem_t *rw_sem = (sem_t *)adam->rw_sem;
    int *diff = adam->diff;

    while (true && !feof(stdin)) {
        sem_wait(rw_sem); // lock writing
            printf("What is your tweet, Adam?\n");
            fgets(tweet_buff, TWEET_BUFF_SIZE, stdin); //write
            (*diff)++; //change message number
        sem_post(rw_sem); // unlock writing
    }
}

int main (int argc, char **argv)
{
    char input_buff[INPUT_BUFF_SIZE];
    char tweet_buff[TWEET_BUFF_SIZE];
    int friend_num = 0;

    int read_count = 0;
    int diff = 0;
    sem_t rw_sem; // semaphore for readers and writer
    sem_init(&rw_sem, 0, 1); // init to only allow 1 past wait
    sem_t r_sem; // semaphore for readers
    sem_init(&r_sem, 0, 1); // init to only allow 1 past wait

    printf("How many friends should tweet, Adam? ");
    int tn_stat = scanf("%d", &friend_num); // get friend_num
    while (fgetc(stdin) != '\n'); // Read until a newline is found
    /* Check for scanf errors */
    if (tn_stat == EOF) {
        if (!errno) {
            printf("EOF reached, exiting...\n");
            exit(0);
        } else {
            perror("EOF reached, exiting...");
            exit(-1);
        }
    }
    if (friend_num <= 0) {
        printf("No friends this time, huh?\nExiting...\nGoobye Adam!\n");
        exit(0);
    }

    /* get all the friends' names */
    printf("What are your friend's names(separated by spaces), Adam?\n");
    fgets(input_buff, INPUT_BUFF_SIZE, stdin);

    /* split friends list into individual friends */
    struct Friend_List flist;
    flist.head = NULL;
    flist.tail = NULL;
    flist.count = 0;
    split_friends(&flist, input_buff);

    if (flist.count != friend_num) {
        fprintf(stderr, "friend count does not match number entered\n");
        exit(-1);
    }

    /* make a tweeter struct for each friend */
    struct Tweeter tweeters[flist.count];
    Friend_Node *curr = flist.head;
    for (int i = 0; curr != NULL; i++) {
        tweeters[i].name = curr->name;
        tweeters[i].tweet = &tweet_buff;
        tweeters[i].rw_sem = &rw_sem;
        tweeters[i].r_sem = &r_sem;
        tweeters[i].read_count = &read_count;
        tweeters[i].diff = &diff;
        curr = curr->next;
    }

    /* Make Tweeter struct for Adam */
    struct Tweeter adam;
    adam.name = "Adam";
    adam.tweet = &tweet_buff;
    adam.rw_sem = &rw_sem;
    adam.r_sem = &r_sem;
    adam.read_count = &read_count;
    adam.diff = &diff;

    /* init all pthread variables */
    pthread_attr_t pthr_attr;
    pthread_attr_init(&pthr_attr);
    pthread_t tids[friend_num];
    pthread_t tid_adam;

    /* create thread for Adam */
    if (pthread_create(&tid_adam, &pthr_attr, Adam, &adam)) {
        perror("Failed to create thread");
        exit(-1);
    }

    /* create friend threads - free everything and exit on error */
    for (int i = 0; i < friend_num; i++) {
        if (pthread_create(&tids[i], &pthr_attr, friennnd, &tweeters[i])) {
            perror("Failed to create thread");
            exit(-1);
        }
    }

    /* wait on Adam's thread */
    if (pthread_join(tid_adam, NULL)) {
        perror("Failed to join thread");
        exit(-1);
    }

    /* wait on each friend thread */
    for (int i = 0; i < friend_num; i++) {
        if (pthread_join(tids[i], NULL)) {
            perror("Failed to join thread");
            exit(-1);
        }
    }

    /* free everything */
    free_friend_list(&flist);
    pthread_attr_destroy(&pthr_attr);
    sem_destroy(&rw_sem);
    sem_destroy(&r_sem);
    return 0;
}

/**
 * Takes a char* of input friends' names, separated by spaces, and
 * splits them into individual nodes for being put into the linked
 * list tracked by the Friend_List struct
 */
static bool split_friends (struct Friend_List *flist, char *input)
{
    int length = strlen(input);
    char name[length];
    char ch;

    Name_Read_State State = Init_State;

    for (int i = 0, j = 0; i < length; i++) {
        ch = input[i];
        switch (State) {
            /*
             * eats all spaces until a letter is found, then State->Letter
             * anything else throws an error and returns false
             */
            case Init_State:
                if (ch == '\n') {
                    return false;
                } else if (ch == ' ') {
                } else if (32 <= ch && ch <= 127) {
                    State = Letter_State;
                    name[j++] = ch;
                } else {
                    fprintf(stderr, "Unrecognized character %c\n", ch);
                    return false;
                }
                break;
            /*
             * reads all letters in a row, on space change State->Blank
             * if newline/space is found, save name to friend list
             * anything else throws an error and returns false
             */
            case Letter_State:
                if (ch == '\n') {
                    name[j] = '\0';
                    save_friend(name, &flist);
                    j = 0;
                } else if (ch == ' ') {
                    State = Blank_State;
                    name[j] = '\0';
                    save_friend(name, &flist);
                    j = 0;
                } else if (32 <= ch && ch <= 127) {
                    name[j++] = ch;
                } else {
                    fprintf(stderr, "Unrecognized character %c\n", ch);
                    free_friend_list(flist);
                    return false;
                }
                break;
            /*
             * eats all spaces until a letter is found, then State->Letter
             * does nothing on newline, because newline == end of input
             * anything else throws an error and returns false
             */
            case Blank_State:
                if (ch == '\n') {
                } else if (ch == ' ') {
                } else if (32 <= ch && ch <= 127) {
                    State = Letter_State;
                    name[j++] = ch;
                } else {
                    fprintf(stderr, "Unrecognized character %c\n", ch);
                    free_friend_list(flist);
                    return false;
                }
                break;
            default:
                break;
        }
    }
    return true;
}

/**
 * Stores one of Adam's friends at the end of the linked list that
 * the Friend_List struct keeps track of
 */
static void save_friend (char *name, struct Friend_List **flist)
{
    Friend_Node *f_node = malloc(sizeof(Friend_Node)); // make new node
    int length = strlen(name);
    if (f_node == NULL) {
        perror("malloc failed in save_friend");
        exit(-1);
    }
    f_node->name = malloc(sizeof(char)*length+1); // make space for name
    if (f_node->name == NULL) {
        perror("malloc failed in save_friend");
        exit(-1);
    }
    strncpy(f_node->name, name, length+1); // put name in node
    f_node->name[length] = '\0'; // in case strncpy doesn't null terminate
    f_node->next = NULL; // set next to NULL, node is going at the end

    if ((*flist)->head == NULL) { // if head is NULL, new node is head
        (*flist)->head = f_node;
        (*flist)->tail = f_node;
        (*flist)->count = 1;
    } else { // otherwise insert node at the end
        (*flist)->tail->next = f_node;
        (*flist)->tail = f_node;
        (*flist)->count++;
    }
    return;
}

/**
 * Frees the friend's list linked list
 */
static void free_friend_list(struct Friend_List *flist)
{
    Friend_Node *temp = flist->head;
    while (temp != NULL) {
        flist->head = flist->head->next;
        free(temp->name);
        free(temp);
        temp = flist->head;
    }
    flist->head = NULL;
    flist->tail = NULL;
    flist->count = 0;
    return;
}
