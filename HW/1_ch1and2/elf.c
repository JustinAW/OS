#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAGIC_BYTES 4

/**
 * Returns true if the file magic has ELF in it
 */
int isELF (char *bytes)
{
    if (strstr(bytes, "ELF") != NULL) {
        return 1;
    }
    return 0;
}

/**
 * Returns true if the file magic has MZ in it
 */
int isPE (char *bytes)
{
    if (strstr(bytes, "MZ") != NULL) {
        return 1;
    }
    return 0;
}

int main (int argc, char **argv)
{
    char bytes[NUM_MAGIC_BYTES]; // buffer for magic bytes
    if (argc != 2)
    {
        printf("Error - must give me a file name\n");
        exit(-1);
    }

    printf("Checking magic on file named %s\n", argv[1]);

    FILE *fp = fopen(argv[1], "r"); // open given file

    fread(bytes, NUM_MAGIC_BYTES, 1, fp); // read the magic bytes

    if (isELF(bytes)) {
        printf("File magic is ELF\n");
    }
    else if (isPE(bytes)) {
        printf("File magic is PE\n");
    }
    else {
        printf("Unrecognizable magic\n");
    }

    fclose(fp); // close the file

    return 0;
}
