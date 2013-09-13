COPTS = -std=c99 -g -D_GNU_SOURCE

esh: main.o
	cc main.o -o esh
main.o: main.c
	cc  $(COPTS) -c main.c
