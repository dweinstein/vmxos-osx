all: io.o

io.o: io.c io.h
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -o io.o -c io.c

clean:
	rm -f *.o
