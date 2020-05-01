/********************************
 * Homework chapter 11          *
 * Author: Dr. Briggs           *
 * Completed by: Justin Weigle  *
 ********************************/

/**
    Quicksort using mmap() and low-level I/O

    This utility is part of Homework 11.  There are several locations 
    where you are prompted to write the code.  These appear as "TODO" marks
    throughout the file.  They are all using the low-level C routines.
    You do not need to chage any line of code that isn't marked with a TODO.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "data.h"


// Open the file, get its size (in bytes), and then mmap() its contents into the address space
int open_and_map(const char* file_name, record_t **records, unsigned int *num_records)
{
    //TODO open the file_name for read and write
    int fid = open("data.dat", O_RDWR);
    if (fid < 0) {
        perror("Error opening file ");
        exit(-1);
    }

    struct stat statbuf;
    //TODO get the file's status information
    int rc = fstat(fid, &statbuf);
    if (rc < 0) {
        perror("Error fstat()-ing file.");
        exit(-1);
    }

    //TODO get the number of bytes for the file from the statbuf.
    size_t bytes = statbuf.st_size;

    *num_records = bytes / RECORD_SIZE;

    // this works, just remarking - this is the mmap() magic that makes this work
    *records = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fid, 0);
    if (records == MAP_FAILED) {
        perror("Error mapping file");
        exit(-1);
    }

    return fid;
    // don't close the file, its mapped!
}

void close_and_unmap(record_t* records, int fid, unsigned int num_records)
{
    size_t len = (num_records)*RECORD_SIZE;
    munmap(records, len);
    close(fid);
}

// swap two records (a_idx with b_idx) in the records array
void swap_records(record_t* records, int a_idx, int b_idx)
{
    record_t temp;

    //TODO use memcpy to swap the record at index a with index b
    memcpy(&temp, &records[a_idx], sizeof(record_t));
    memcpy(&records[a_idx], &records[b_idx], sizeof(record_t));
    memcpy(&records[b_idx], &temp, sizeof(record_t));
}

int partition(record_t* records, int low, int high) {
    int pivot = high;
    int i = low;
    for (int j = low; j <= high; j++) {
        if (records[j].key < records[pivot].key) {
            swap_records(records, i, j);
            i = i + 1;
        }
    }
    swap_records(records, i, high);
    return i;
}

void quick_sort(record_t* records, int low, int high)
{
    if (low < high) {
        int p = partition(records, low, high);
        quick_sort(records, low, p - 1);
        quick_sort(records, p + 1, high);
    }
}

int main(int argc, char** argv)
{
    record_t* records;
    unsigned int num_records;

    int fid = open_and_map("data.dat", &records, &num_records);
    quick_sort(records, 0, num_records - 1);

    for (int i = 0; i < num_records; i++) {
        printf("%d\n", records[i].key);
    }
    close_and_unmap(records, fid, num_records);
}
