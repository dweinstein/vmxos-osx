all: paging.o

paging.o: paging.c paging.h ../vmx/vmx.h ../kernel.h
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -o paging.o -c paging.c

clean:
	rm -f *.o
