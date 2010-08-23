all: vmx.o vmx_hw.o

vmx.o: vmx.c vmx.h 
	gcc -ffreestanding -fno-builtin -nostdlib -o vmx.o -c vmx.c

vmx_hw.o: vmx_hw.asm
	nasm -f aout vmx_hw.asm -o vmx_hw.o

clean:
	del *.o
