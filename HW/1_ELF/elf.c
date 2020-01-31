#include <stdio.h>
#include <stdlib.h>

#define NUM_MAGIC_BYTES 1

int isELF (char *bytes)
{
    return 0;
}

int isPE (char *bytes)
{
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

    //fopen(file on argv[1])

    //fread(NUM_MAGIC_BYTES)

    if (isELF(bytes))
    {
        printf("File is ELF\n");
    }
    else if (isPE(bytes))
    {
        printf("File is PE\n");
    }

    //fclose(file)

    return 0;
}
