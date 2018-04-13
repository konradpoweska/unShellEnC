GCC_FLAGS = -Wall -g

all: toto mini-shell

mini-shell: mini-shell.c
	gcc ${GCC_FLAGS} -o mini-shell mini-shell.c

toto: toto.c
	gcc ${GCC_FLAGS} -o toto toto.c

clean:
	rm toto mini-shell
