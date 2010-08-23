#include "io.h"

/* Outputs a byte to the specified hardware port */
inline void outportb(unsigned int port, unsigned char value)
{
	asm volatile("outb %%al,%%dx"::"d" (port), "a" (value));
}

/* Outputs a word to a port */
inline void outportw(unsigned int port, unsigned int value)
{
	asm volatile("outw %%ax,%%dx"::"d" (port), "a" (value));
}

/* gets a byte from a port */
inline unsigned char inportb(unsigned int port)
{
	unsigned char value;
	asm volatile("inb %w1,%b0" : "=a"(value) : "d"(port));
	return value;
}
