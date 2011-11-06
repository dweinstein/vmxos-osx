all: main.o start.o
	make -C console
	make -C vmx
	make -C paging
	make -C A20
	make -C interrupt
	make -C io
	ld -T link.ld -o kernel.bin start.o main.o console/*.o vmx/*.o paging/*.o A20/*.o interrupt/*.o io/*.o

start.o: start.asm
#	nasm -f coff start.asm -o start.o
	nasm -f aout start.asm -o start.o
#	nasm start.asm -o start.o


main.o: main.c main.h vmx/vmx.h console/console.h paging/paging.h A20/A20.h interrupt/interrupt.h ../version.h ../build.h
	gcc -ffreestanding -fno-builtin -nostdlib -c main.c

clean:
	find . -name '*.o' -exec rm -f {} \;
	make -C console clean
	make -C vmx clean
