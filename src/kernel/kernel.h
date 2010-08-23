#ifndef _KERNEL_H_
#define _KERNEL_H_

// converts to unsigned int
#define UINT(T) *(unsigned int *) &T

// hlt defined in start.asm
extern void hlt();

#endif
