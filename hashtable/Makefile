CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -g
FILES=hashtable.c test.c test_util.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
