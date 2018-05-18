GCC_FLAGS = -Wall -g

all: mini-shell

mini-shell: mini-shell.o util.o builtin.o
	gcc ${GCC_FLAGS} -o mini-shell mini-shell.o util.o builtin.o

mini-shell.o: mini-shell.c
	gcc ${GCC_FLAGS} -c mini-shell.c

util.o: util.c util.h
	gcc ${GCC_FLAGS} -c util.c

builtin.o: builtin.c builtin.h
	gcc ${GCC_FLAGS} -c builtin.c

clean:
	rm *.o mini-shell
