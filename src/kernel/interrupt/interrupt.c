#include "interrupt.h"

IDTR_t idtr;
IDT_t idt[256];

char* exceptions[] = 
{
	"00 #00 Divide By Zero Error",
	"01 #DB Debug Error",
	"02 #-- NMI Interrupt",
	"03 #BP Breakpoint",
	"04 #OF Overflow",
	"05 #BR BOUND Range Exceeded",
	"06 #UD Invalid Opcode",
	"07 #NM Device Not Available",
	"08 #DF Double Fault",
	"09 #-- Coprocessor Segment Overrun",
	"10 #TS Invalid TSS",
	"11 #NP Segment Not Present",
	"12 #SS Stack Segment Fault",
	"13 #GP Gneral Protection Fault",
	"14 #PF Page Fault",
	"15 #15 reserved",
	"16 #MF FPU Floating-Point Exception",
	"17 #AC Alignment Check",
	"18 #MC Machine Check",
	"19 #XF SIMD Floating-Point Exception",
	"20 #20 reserved",
	"21 #21 reserved",
	"22 #22 reserved",
	"23 #23 reserved",
	"24 #24 reserved",
	"25 #25 reserved",
	"26 #26 reserved",
	"27 #27 reserved",
	"28 #28 reserved",
	"29 #29 reserved",
	"30 #SX Security Exception",
	"31 #31 reserved",
};

void load_idtr()
{
    idtr.limit  = 256*(sizeof(IDT_t)-1);
    idtr.base   = idt;

	// load IDTR with lidt
	lidt(&idtr);
}

void set_gate(int index, void (*handler)())
{
     unsigned int offset = (unsigned int)handler;
	 
	 idt[index].low_offset   = (offset & 0xFFFF);
	 idt[index].selector     = 0x08;
	 idt[index].settings     = INT_0;
     idt[index].high_offset  = (offset >> 16);
}

void init_exceptions()
{
	int i = 0;
	
	// add all Exception Interrupts
	set_gate(0, int0);
	set_gate(1, int1);
	set_gate(2, int2);
	set_gate(3, int3);
	set_gate(4, int4);
	set_gate(5, int5);
	set_gate(6, int6);
	set_gate(7, int7);
	set_gate(8, int8);
	set_gate(9, int9);
	set_gate(10, int10);
	set_gate(11, int11);
	set_gate(12, int12);
	set_gate(13, int13);
	set_gate(14, int14);
	set_gate(15, int15);
	set_gate(16, int16);
	set_gate(17, int17);
	set_gate(18, int18);
	set_gate(19, int19);
	set_gate(20, int20);
	set_gate(21, int21);
	set_gate(22, int22);
	set_gate(23, int23);
	set_gate(24, int24);
	set_gate(25, int25);
	set_gate(26, int26);
	set_gate(27, int27);
	set_gate(28, int28);
	set_gate(29, int29);
	set_gate(30, int30);
	set_gate(31, int31);
	i=32;
	for (; i < 48; i++) set_gate(i, hwint);
	set_gate(48, int48);

	load_idtr();
}

void remap_pic(int pic1, int pic2)
{
	unsigned char md, sd;

	md = inportb(PIC_MASTERDATA);                   // save state of MASTER DATA
	sd = inportb(PIC_SLAVEDATA);                    // save state of SLAVE DATA

	outportb(PIC_MASTER, PIC_EOI);                  // Send EOI | resets the chip

	outportb(PIC_MASTER, ICW1_INIT+ICW1_ICW4);      // ICW1 control word setup | just basic PIC stuff
	outportb(PIC_SLAVE, ICW1_INIT+ICW1_ICW4);       // see pic.h for more details about the values

	outportb(PIC_MASTERDATA, pic1);                 // ICW2 maps IRQs 0-7 to whatever kernel passes
	outportb(PIC_SLAVEDATA, pic2);                  // and same here except with IRQs 8-15 

	outportb(PIC_MASTERDATA, 0x04);                 // ICW3 
	outportb(PIC_SLAVEDATA, 0x02);

	outportb(PIC_MASTERDATA, ICW4_8086);            // ICW4 control word setup
	outportb(PIC_SLAVEDATA, ICW4_8086);

	outportb(PIC_MASTERDATA, md);                   // restore both MASTER DATA
	outportb(PIC_SLAVEDATA, sd);                    // restore SLAVE DATA
}

// makes IRQ unavailable so it can't fire an interrupt
void mask_irq(unsigned char irq)
{
	if(irq == IRQ_ALL)
	{
		outportb(PIC_MASTERDATA, 0xFF);
		outportb(PIC_SLAVEDATA, 0xFF);
	}
	else
	{
		irq = irq | (1<<irq);
		if(irq < 8)
			outportb(PIC_MASTERDATA, irq & 0xFF);
		else
			outportb(PIC_SLAVEDATA, irq >> 8);
	}
}

void unmask_irq(unsigned char irq)
{
	if(irq == IRQ_ALL)
	{
		outportb(PIC_MASTERDATA, 0x00);
		outportb(PIC_SLAVEDATA, 0x00);
	}
	else
	{
		irq = irq & (1<<irq);
		if(irq < 8)
			outportb(PIC_MASTERDATA, irq & 0xFF);
		else
			outportb(PIC_SLAVEDATA, irq >> 8);
	}
}

void syscall_handler(unsigned int code)
{
	puts("\nsyscall "); puts(hex2string(code));
}

void exception_handler(unsigned int code)
{
	puts("\n"); puts(dec2string(code)); puts(exceptions[code]);
	hlt();
}

void exception_handler_err(unsigned int code, unsigned int err)
{
	puts("\n"); puts(exceptions[code]); puts(" -> error code: "); puts(bin2string(err));
	switch(code)
	{
		case 14: // page fault
			puts("\nCR2   : "); puts(hex2string(vmx_read_cr2())); puts(" = "); puts(bin2string(vmx_read_cr2())); puts("\n");
			break;
	}
	hlt();
}

void hwint_handler()
{
    unsigned char scancode;
	char s[2];

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    else
    {
		s[0] = kbdus[scancode];
		s[1] = 0;
        puts(s);
    }
	outportb(PIC_MASTER, PIC_EOI);
}
