#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "../io/io.h"

extern unsigned char kbdus[128];

// clear screen
extern void cls();

// prints a string on the screen
extern void puts(char *message);

// converts number into hex format string
extern char* hex2string(int val);

// converts number into binary format string
extern char* bin2string(int val);

// converts number into decimal format string
extern char* dec2string(int val);

// scroll down screen
extern void scroll();

// updates the hardware cursor.
extern void move_cursor();

#endif
