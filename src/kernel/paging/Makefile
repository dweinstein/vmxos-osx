all: paging.o

paging.o: paging.c paging.h ../vmx/vmx.h ../kernel.h
	gcc -ffreestanding -fno-builtin -nostdlib -o paging.o -c paging.c

clean:
	del *.o
