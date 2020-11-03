CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: hash.o

hash.o: hash.c
	$(CC) $(CFLAGS) -g -c hash.c

htest1.o: htest1.c hash.h
	$(CC) $(CFLAGS) -g -c htest1.c

htest1: htest1.o hash.o
	$(CC) $(CFLAGS) -g htest1.o hash.o -o htest1

run: htest1
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./htest1
	./htest1

clean:
	rm *.o

cleanall:
	rm *.o htest1
