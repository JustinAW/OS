CFLAGS=-ggdb -std=gnu11
LDFLAGS=-ggdb -std=gnu11
CC=gcc

all: generate mmap seek

.c.o:
	$(CC) $(CFLAGS) -c $<

generate: generate.o
	$(CC) -o generate generate.o $(LDFLAGS)

mmap: mmap.o
	$(CC) -o mmap mmap.o $(LDFLAGS)

seek: seek.o
	$(CC) -o seek seek.o $(LDFLAGS)


clean:
	rm -f *.o generate mmap