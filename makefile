CC=gcc
CFLAGS=-Wall -ansi

contactsList: contactsList.o
	$(CC) -o contactsList contactsList.o

contactsList.o : contactsList.c
	$(CC) $(CFLAGS) -c contactsList.c

clean :
	rm contactsList contactsList.o
