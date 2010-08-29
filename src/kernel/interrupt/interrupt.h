#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "../kernel.h"
#include "../console/console.h"
#include "../io/io.h"
#include "../vmx/vmx.h"

// interrupt types
#define INT_0 0x8E00	// 1000111000000000 = present, ring0, int_gate
#define INT_3 0xEE00	// 1110111000000000 = present, ring3, int_gate

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

// interrupt handlers
void syscall_handler(unsigned int);
void exception_handler(unsigned int);
void exception_handler_err(unsigned int, unsigned int);
void hwint_handler();

extern void init_exceptions();
extern void invoke48();
extern void lidt(unsigned int);

extern void remap_pic(int pic1, int pic2);
extern void mask_irq(unsigned char irq);
extern void unmask_irq(unsigned char irq);

// cli e sti
extern void cli();
extern void sti();

// interrupt gates
extern void int0();
extern void int1();
extern void int2();
extern void int3();
extern void int4();
extern void int5();
extern void int6();
extern void int7();
extern void int8();
extern void int9();
extern void int10();
extern void int11();
extern void int12();
extern void int13();
extern void int14();
extern void int15();
extern void int16();
extern void int17();
extern void int17();
extern void int18();
extern void int19();
extern void int20();
extern void int21();
extern void int22();
extern void int23();
extern void int24();
extern void int25();
extern void int26();
extern void int27();
extern void int28();
extern void int29();
extern void int30();
extern void int31();

extern void int48();
extern void hwint();

#endif
