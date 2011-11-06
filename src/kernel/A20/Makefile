all: A20_hw.o

A20_hw.o: A20_hw.asm
	nasm -f aout A20_hw.asm -o A20_hw.o
#	nasm -f coff A20_hw.asm -o A20_hw.o

clean:
	rm -f  *.o
