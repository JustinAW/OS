#ifndef _DATA_H
#define _DATA_H

#include <stdint.h>

#define RECORD_SIZE 512
#define KEY_DENSITY 20000
#define NUM_RECORDS 10

typedef struct {
	uint32_t key;
	uint32_t num;
	char data[RECORD_SIZE - 8];
} record_t;

#endif