CC=gcc
CFLAGS=-Werror

all: find

find: find.o main.c kmp.c

test: test_kmp

test_kmp: kmp.o test/test_kmp.o
	rm -f $@
	$(CC) $(CFLAGS) $? -o $@
	./$@

clean:
	rm -f test_kmp *.o test/*.o 

.PHONY: test
.PHONY: clean