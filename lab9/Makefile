CC=gcc
CFLAGS= -O2 -std=gnu99

all: sum
sum: sum.c
	$(CC) $(CFLAGS) -o sum sum.c

sseTest.s : sseTest.c
	$(CC) -S sseTest.c

clean:
	rm -rf sum
