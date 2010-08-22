all: vmx_hw.o

vmx_hw.o: vmx_hw.asm
	nasm -f aout vmx_hw.asm -o vmx_hw.o

clean:
	del *.o
