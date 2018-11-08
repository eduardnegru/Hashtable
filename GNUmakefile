CC=gcc
CFLAGS=-Wall -fPIC

build:	tema1

tema1:	tema1.o
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
	gcc -o tema1 tema1.o -L. -lhash

tema1.o:	tema1.c
	gcc -c tema1.c

clean:
	rm -f *.o
	rm -f tema1
