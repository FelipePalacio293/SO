CC = gcc
%.o: %c
	$(CC) -c -o $@ $<
allshell: shell
all: shell
Proyecto1.c: shell.o
	gcc -o shell
	shell.o
clean: 
	rm -f *.o shell