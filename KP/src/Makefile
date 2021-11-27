CC=gcc
CFLAGS=-pedantic -Wall

all: a b c

a: a.o
	gcc a.c -o a

b: b.o
	gcc b.c -o b

c: c.o
	gcc c.c -o c

clean:
	rm a b c *.o