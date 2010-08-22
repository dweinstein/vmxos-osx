all: io.o

io.o: io.c io.h
	gcc -ffreestanding -fno-builtin -nostdlib -o io.o -c io.c

clean:
	del *.o
