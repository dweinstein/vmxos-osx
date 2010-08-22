all: console.o

console.o: console.c console.h
	gcc -ffreestanding -fno-builtin -nostdlib -o console.o -c console.c

clean:
	del *.o
