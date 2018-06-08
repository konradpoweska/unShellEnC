GCC_FLAGS = -Wall -g

all: mini-shell

mini-shell: mini-shell.o util.o builtin.o
	gcc ${GCC_FLAGS} -o mini-shell mini-shell.o util.o builtin.o

mini-shell.o: mini-shell.c
	gcc ${GCC_FLAGS} -c mini-shell.c

util.o: util.c util.h
	gcc ${GCC_FLAGS} -c util.c

input_parser.o: input_parser.c input_parser.h
	gcc ${GCC_FLAGS} -c input_parser.c

builtin.o: builtin.c builtin.h
	gcc ${GCC_FLAGS} -c builtin.c

redirect-io.o: redirect-io.c redirect-io.h
	gcc ${GCC_FLAGS} -c redirect-io.c

execution.o: execution.c execution.h
	gcc ${GCC_FLAGS} -c execution.c

clean:
	rm *.o mini-shell
