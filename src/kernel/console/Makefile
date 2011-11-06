all: console.o

console.o: console.c console.h
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -o console.o -c console.c

clean:
	rm -f *.o
