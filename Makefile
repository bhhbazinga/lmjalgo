CC = gcc
CFLAGS = -g -O0 -Wall -pedantic
SHARED = -fPIC --shared

all : lmjalgo.so

lmjalgo.so : lmjalgo.c mjalgo.c mjalgo.h
	$(CC) $(CFLAGS) $(SHARED) -o $@ $^

clean :
	rm *.so

.PHONY :
	clean
