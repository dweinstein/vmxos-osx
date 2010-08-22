#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "../console/console.h"
#include "../io/io.h"

// interrupt types
#define INT_0 0x8E00						// 1000111000000000 = present, ring0, int_gate
#define INT_3 0xEE00						// 1110111000000000 = present, ring3, int_gate

/*
 * IRQ macros
 */
#define IRQ_ALL       0xFF
#define IRQ_TIMER     0
#define IRQ_KEYBOARD  1
#define IRQ_CASCADE   2
#define IRQ_COM2_4    3
#define IRQ_COM1_3    4
#define IRQ_LPT       5
#define IRQ_FLOPPY    6
#define IRQ_FREE7     7
#define IRQ_CLOCK     8
#define IRQ_FREE9     9
#define IRQ_FREE10    10
#define IRQ_FREE11    11
#define IRQ_PS2MOUSE  12
#define IRQ_COPROC    13
#define IRQ_IDE_1     14
#define IRQ_IDE_2     15

/*
 * PIC macros
 * Master = PIC chip 1
 * Slave = PIC chip 2
 */
#define PIC_MASTER          0x20
#define PIC_MASTERDATA      0x21
#define PIC_SLAVE           0xA0
#define PIC_SLAVEDATA       0xA1
#define PIC_EOI             0x20

// ICW1 and ICW4 control words are the only ones we have to worry about
#define ICW1_INIT    0x10               // required for PIC initialisation
#define ICW1_EDGE    0x08               // edge triggered IRQs
#define ICW1_SINGLE  0x02               // only MASTER (not cascaded)
#define	ICW1_ICW4    0x01               // there IS an ICW4 control word

#define ICW4_SFNM    0x10               // Special Fully Nested Mode
#define ICW4_BUFFER  0x08               // Buffered Mode
#define ICW4_MASTER  0x04               // this is the Master PIC
#define ICW4_AEOI    0x02               // Auto EOI
#define ICW4_8086    0x01               // 80/86 Mode

// structure for an interrupt
typedef struct
{
	unsigned short low_offset;				// low nibble of offset to handler of interrupt 
	unsigned short selector;				// GDT selector used 
	unsigned short settings;				// settings for int 
	unsigned short high_offset;				// high nibble to handler code 
} __attribute__ ((packed)) IDT_t;


// structure for the IDTR
typedef struct
{
     unsigned short limit;                  // limit or Size of IDT 
     unsigned int base;			            // a pointer to the base of the IDT 
} __attribute__ ((packed)) IDTR_t;

extern void generic_int();
extern void int48();
extern void generic_pic();

extern void init_exceptions();
extern void invoke48();
extern void lidt(unsigned int);

extern void remap_pic(int pic1, int pic2);
extern void mask_irq(unsigned char irq);
extern void unmask_irq(unsigned char irq);

// cli e sti
extern void cli();
extern void sti();

#endif
