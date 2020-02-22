#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <pthread.h>

typedef struct {
    const char* pattern;
    const char* filename;
} thread_args_t;

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
    FILE* fp = fopen(pathname,"r");             // open the file
    if (fp == NULL)                             // see if it failed
    {
        fprintf(stderr, "Error - could not open file %s", strerror(errno));
        exit(-1);
    }

    char buff[1024];                            // create a buff to hold a line

    while (1) {
        memset(buff, 0, sizeof(buff));          // zero out the buff
        char *ptr = fgets(buff, sizeof(buff)-1, fp);    // read the line
        if (ptr == NULL)                        // check for end of file / error
            break;                              // break if that happened

        if (matches(reg, buff)) {               // check if the line matches the pattern
            puts(buff);                         // if so, print it
        }
    }
}

void run(void* arg)
{
    regex_t reg;

    thread_args_t* thread_args = (thread_args_t*)arg;
    compile(&reg, thread_args->pattern);
    handle_file(&reg, thread_args->filename);
    delete(&reg);
}


/** main() - read the files identified by the argv list, check if they match the pattern. */
int main(int argc, char **argv)
{
    int i;

    if (argc < 3) {
        fprintf(stderr, "error - run as %s pattern file1 ... filen\n",
            argv[0]);
    }

    pthread_attr_t attrs;
    pthread_t pids[argc-2];
    thread_args_t args[argc - 2];
    for (i = 2; i < argc; i++) {

        args[i-2].filename = argv[i];
        args[i-2].pattern = argv[1];

        pthread_attr_init(&attrs);
        pthread_create(&pids[i - 2], &attrs, run, &args[i-2]);
    }
}
