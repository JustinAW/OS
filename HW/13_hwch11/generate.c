#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define RECORD_SIZE 512
#define KEY_DENSITY 20000
#define NUM_RECORDS 1000

typedef struct {
    uint32_t key;
    uint32_t num;
    char data[RECORD_SIZE - 8];
} record_t ;

void create_record(FILE* fp)
{
    static int record_num = 0;
    record_t record;

    record.key = (uint32_t)random() % KEY_DENSITY;
    record.num = record_num++;

    for (int i = 0; i < (RECORD_SIZE - 8); i++) {
        record.data[i] = 'A' + (record_num % 26);
    }

    fwrite(&record, sizeof(record), 1, fp);
}

int main(int argc, char** argv)
{
    srandom(time(NULL));

    FILE* fp = fopen("data.dat", "w+");
    for (int i = 0; i < NUM_RECORDS; i++) {
        create_record(fp);
    }
    fflush(fp);
    fclose(fp);
}
