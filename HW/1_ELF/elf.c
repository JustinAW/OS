#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAGIC_BYTES 4

int isELF (char *bytes)
{
    if (strstr(bytes, "ELF") != NULL) {
        return 1;
    }
    return 0;
}

int isPE (char *bytes)
{
    if (strstr(bytes, "MZ") != NULL) {
        return 1;
    }
    return 0;
}

int main (int argc, char **argv)
{
    char bytes[NUM_MAGIC_BYTES];

    if (argc != 2)
    {
        printf("Error - must give me a file name\n");
        exit(-1);
    }

    printf("Checking magic on file named %s\n", argv[1]);

    FILE *fp = fopen(argv[1], "r");

    fread(bytes, NUM_MAGIC_BYTES, 1, fp);

    if (isELF(bytes)) {
        printf("File is ELF\n");
    }
    else if (isPE(bytes)) {
        printf("File is PE\n");
    }
    else {
        printf("File is neither ELF nor PE\n");
    }

    fclose(fp);

    return 0;
}
