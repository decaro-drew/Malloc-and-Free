all: memgrind

memgrind: mymalloc.o memgrind.o
	gcc -g -o memgrind mymalloc.o memgrind.o

memgrind.o: memgrind.c mymalloc.h
	gcc -g -c memgrind.c mymalloc.h

mymalloc.o: mymalloc.c
	gcc -g -c mymalloc.c



clean:
	-rm -rf memgrind mymalloc.o memgrind.o mymalloc.h.gch
