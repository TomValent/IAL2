CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -g
FILES=btree.c ../btree.c stack.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
