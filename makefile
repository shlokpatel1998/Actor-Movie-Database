CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: hash.o

hash.o: hash.c
	$(CC) $(CFLAGS) -g -c hash.c

htest1.o: htest1.c hash.h
	$(CC) $(CFLAGS) -g -c htest1.c

htest1: htest1.o hash.o
	$(CC) $(CFLAGS) -g htest1.o hash.o -o htest1

htest2.o: htest2.c hash.h
	$(CC) $(CFLAGS) -g -c htest2.c

htest2: htest2.o hash.o
	$(CC) $(CFLAGS) -g htest2.o hash.o -o htest2

htest3.o: htest3.c hash.h
	$(CC) $(CFLAGS) -g -c htest3.c

htest3: htest3.o hash.o
	$(CC) $(CFLAGS) -g htest3.o hash.o -o htest3

htest4.o: htest4.c hash.h
	$(CC) $(CFLAGS) -g -c htest4.c

htest4: htest4.o hash.o
	$(CC) $(CFLAGS) -g htest4.o hash.o -o htest4

run1: htest1
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./htest1
	./htest1

run2: htest2
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./htest2
	./htest2

run3: htest3
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./htest3
	./htest3

run4: htest4
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./htest4
	./htest4

clean:
	rm *.o

cleanall:
	rm *.o htest1 htest2 htest3 htest4
