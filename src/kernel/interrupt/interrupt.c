#include "interrupt.h"

IDTR_t idtr;
IDT_t idt[256];

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
	for (; i < 32; i++) set_gate(i, generic_int);
	for (; i < 48; i++) set_gate(i, generic_pic);
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

void int_48()
{
	puts("int40");
}

void generic_int_handler()
{
	puts("int");
	outportb(PIC_MASTER, PIC_EOI);
	sti();
}

void generic_pic_handler()
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
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
		s[0] = kbdus[scancode];
		s[1] = 0;
        puts(s);
    }
	outportb(PIC_MASTER, PIC_EOI);
	sti();
}
