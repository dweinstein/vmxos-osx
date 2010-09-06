#ifndef _KERNEL_H_
#define _KERNEL_H_

// converts to unsigned int
#define UINT(T) *(unsigned int *) &T

// hlt defined in start.asm
extern void hlt();

// jump to a known linear address to let bochs interrupt and debug
extern void break_point();

#endif
