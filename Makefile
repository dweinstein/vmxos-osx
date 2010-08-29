all: 
	make -C src/boot
	make -C src/kernel
	dd if=/dev/zero of=bochs/floppy.img bs=512 count=2880
	dd if=src/boot/boot.bin of=bochs/floppy.img count=1
	dd if=src/kernel/kernel.bin of=bochs/floppy.img seek=1

bochs: all
	cd bochs
	bochs -q

vmware: all
	vmware.bat

clean:
	make -C src/boot clean
	make -C src/kernel clean
