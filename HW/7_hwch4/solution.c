#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <pthread.h>

typedef struct {
    const char *regexp;
    const char *dirpath;
    int t_num;
} t_arg_t;

/* compile() - compile the regular expression.  This
must be done once before the matcher can be used.  Only
one "matcher" can use this regular expression at one time. */
int compile(regex_t* reg, const char* pattern)
{
    return regcomp(reg, pattern, 0);
}

/* delete() - free the memory allocated during compile() */
void delete(regex_t* reg)
{
    regfree(reg);
}

/* matches() - returns true if the given line matches the regular expression.
reg - the compiled regular expression pattern, and
line - the line of text to match. */
int matches(regex_t* reg, const char* line)
{
    int rc = regexec(reg, line, 0, NULL, 0);
    if (rc == 0) return 1;
    else return 0;
}

/* handle_file - read the text, line by line,
and print those lines that match the pattern.
reg - the compiled regular expression pattern, and
line - the line of the text to match. */
void handle_file(regex_t *reg, const char* pathname)
{
    FILE* fp = fopen(pathname,"r");    // open the file
    if (fp == NULL) {                  // see if it failed
        fprintf(stderr, "Error - could not open file %s", strerror(errno));
        exit(-1);
    }

    char buff[1024];                   // create a buff to hold a line

    while (1) {
        memset(buff, 0, sizeof(buff)); // zero out the buff
        char *ptr = fgets(buff, sizeof(buff)-1, fp);    // read the line
        if (ptr == NULL) {             // check for end of file / error
            break;                     // break if that happened
        }
        if (matches(reg, buff)) {      // check if the line matches the pattern
            puts(buff);                // if so, print it
        }
    }
}

/**
 * Called once by each thread created in main,
 * allowing handle_file to be run in parallel
 * for each file argument
 */
void *routine(void *t_args)
{
    t_arg_t *t_arg = (t_arg_t*)t_args; // cast args back to struct
    regex_t regexp;

    printf("Thread number %d searching...\n", (*t_arg).t_num);
    compile(&regexp, (*t_arg).regexp);
    handle_file(&regexp, (*t_arg).dirpath);
    delete(&regexp);
}

/** main() - read the files identified by the argv list,
 * check if they match the pattern. */
int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "error - run as %s pattern file1 ... filen\n",
            argv[0]);
    }
    int n_files = argc - 2;
    pthread_t pid[n_files];
    pthread_attr_t attr;
    t_arg_t t_args[n_files];    // structs for arg storage
    int pt_attr_ret, pt_ret;    // for return vals of attr init and create

    for (int i = 2; i < argc; i++) {
        t_args[i-2].regexp = argv[1];
        t_args[i-2].dirpath = argv[i];
        t_args[i-2].t_num = i-2; //track thread nums, just because

        /***** init thread attrs *****/
        pt_attr_ret = pthread_attr_init(&attr);
        if (pt_attr_ret) { //err if not 0
            fprintf(stderr, "pthread_attr_init failed - error code %d\n",
                    pt_attr_ret);
        }
        /***** create threads *****/
        pt_ret = pthread_create(&pid[i-2], &attr, routine, &t_args[i-2]);
        if (pt_ret) { //err if not 0
            fprintf(stderr, "pthread_create failed - error code: %d\n",
                    pt_ret);
        }
    }
    /***** join threads (wait) *****/
    for (int i = 2; i < argc; i++) {
        pthread_join(pid[i-2], NULL); // wait for the threads to finish
    }

    return 0;
}

