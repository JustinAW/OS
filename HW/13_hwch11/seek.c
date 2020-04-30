/**
    Quicksort using random I/O

    This utility is part of Homework 11. There are several locations
    where you are prompted to write the code.  These appear as "TODO" remarks
    throughout the file.  They are all using the stream I/O routines.  
    You do not need to change any line of code that isn't marked with a TODO.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "data.h"

// reads the nth record from the data file
void read_record(FILE* fp, record_t* rec, int index)
{
    //TODO replace with the call to fseek() that will seek to the nth record (by index)
    // done
    int rc = fseek(fp, index, SEEK_SET);
    if (rc < 0) { perror("seek failed"); exit(-1); }

    //TODO replace with the call to fread() one record into the struct 
    // done
    rc = fread(rec, RECORD_SIZE, (size_t)1, fp);
    if (rc <= 0) { perror("read failed"); exit(-1); }
}

// writes the record to the nth position in the file
void write_record(FILE* fp, record_t* rec, int num)
{
    //TODO seek to the proper byte address
    //TODO write the given record (see read_record for a hint)
}


// Given two indices in the file, swap records a & b (by index)
void swap_records(FILE *fp, int a_idx, int b_idx)
{
    if (a_idx == b_idx) return;
    record_t rec_a, rec_b;

    //TODO read the record from a_idx into rec_a
    //TODO read the record from b_idx into rec_b
    //TODO write the record in rec_a to b_idx
    //TODO write the record in rec_b to a_idx
}

// Quicksort partition the range of data by low and high idx
int partition(FILE *fp, int low, int high) {
    int pivot = high;
    int i = low;
    record_t rec_pivot;
    read_record(fp, &rec_pivot, pivot);

    for (int j = low; j <= high; j++) {

        record_t rec_j;
        read_record(fp, &rec_j, j);

        if (rec_j.key < rec_pivot.key) {
            swap_records(fp, j, i);
            i = i + 1;
        }
    }
    swap_records(fp, i, high);
    return i;
}

// Quick sort the range of data by low and high idx
void quick_sort(FILE *fp, int low, int high)
{
    if (low < high) {
        int p = partition(fp, low, high);
        quick_sort(fp, low, p - 1);
        quick_sort(fp, p + 1, high);
    }
}

// sort the data
int main(int argc, char** argv)
{
    //TODO replace with a call to fopen the file for reading AND writing
    // done
    FILE* fp = fopen("data.dat", "r+");

    // get the file's directory infomration
    struct stat stat_buf;
    fstat(fp->_fileno, &stat_buf);

    //TODO divide the file's size (from stat_buf) by RECORD_SIZE to get # records
    // done
    int num_records = stat_buf.st_size / RECORD_SIZE;

    printf("Sorting %d records\n", num_records);
    quick_sort(fp, 0, num_records - 1);
    for (int i = 0; i < num_records; i++) {
        record_t rec;
        read_record(fp, &rec, i);
        printf("%d\n", rec.key);
    }

    //TODO close the file using fclose
    // done
    fclose(fp);
}
