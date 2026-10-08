CC = gcc
CFLAGS = -Wall -Wextra -g

shell: shell.o parse.o exec.o
	$(CC) $(CFLAGS) -o shell shell.o parse.o exec.o

%.o: %.c parse.h exec.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f shell *.o