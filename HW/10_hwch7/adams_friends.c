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

typedef enum {
    Init_State,
    Letter_State,
    Blank_State
} Name_Read_State;

typedef struct Friend_Node {
    char *name;
    struct Friend_Node *next;
} Friend_Node;

struct Friend_List {
    Friend_Node *head;
    Friend_Node *tail;
    int count;
};

static bool split_friends (struct Friend_List *, char *);
static void save_friend (char *, struct Friend_List **);
static void free_friend_list(struct Friend_List *);
static void print_names (Friend_Node *);

void* friennnd (void *friend) {
    Friend_Node *this_friend = (Friend_Node*)friend;
    printf("%s\n", this_friend->name);
}

int main (int argc, char **argv)
{
    char input_buff[INPUT_BUFF_SIZE];
    char tweet_buff[TWEET_BUFF_SIZE];
    int friend_num = 0;

    printf("How many friends should tweet, Adam? ");
    int tn_stat = scanf("%d", &friend_num); // get friend_num
    while (fgetc(stdin) != '\n'); // Read until a newline is found
    /* Check for scanf errors */
    if (tn_stat == EOF) {
        if (!errno) {
            printf("EOF reached, exiting...\n");
        } else {
            perror("EOF reached, exiting...");
            exit(-1);
        }
    }
    if (friend_num <= 0) {
        printf("No friends this time, huh?\nExiting...\nGoobye Adam!\n");
        exit(0);
    }

    /* get all the friend's names */
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

    /* make a friends array */
    Friend_Node *farr[flist.count];
    Friend_Node *curr = flist.head;
    for (int i = 0; curr != NULL; i++) {
        farr[i] = curr;
        curr = curr->next;
    }

    /* init all pthread variables */
    pthread_attr_t pthr_attr;
    pthread_attr_init(&pthr_attr);
    pthread_t tids[friend_num];

    while (true && !feof(stdin)) {
        printf("What is the tweet?\n");
        fgets(tweet_buff, TWEET_BUFF_SIZE, stdin);
        /* create threads */
        for (int i = 0; i < friend_num; i++) {
            if (pthread_create(&tids[i], &pthr_attr, friennnd, farr[i])) {
                perror("Failed to create thread");
                pthread_attr_destroy(&pthr_attr);
                exit(-1);
            }
        }
        /* wait on each thread */
        for (int i = 0; i < friend_num; i++) {
            if (pthread_join(tids[i], NULL)) {
                perror("Failed to join thread");
                pthread_attr_destroy(&pthr_attr);
                exit(-1);
            }
        }
    }

    free_friend_list(&flist);
    pthread_attr_destroy(&pthr_attr);
    return 0;
}

static bool split_friends (struct Friend_List *flist, char *input)
{
    int length = strlen(input);
    char name[length];
    char ch;

    Name_Read_State State = Init_State;

    for (int i = 0, j = 0; i < length; i++) {
        ch = input[i];
        switch (State) {
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

static void print_names (Friend_Node *head)
{
    Friend_Node *curr = head;
    while (curr != NULL) {
        printf("%s\n", curr->name);
        curr = curr->next;
    }
}
