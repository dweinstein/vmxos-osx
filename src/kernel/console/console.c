#include "console.h"

int console_w = 80,
	console_h = 25,
	cursor_x = 0, 
	cursor_y = 0;

unsigned char console_forecolour = 0xF;
unsigned char console_backcolour = 0x0;
unsigned char kbdus[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};		

// clear screen
void cls()
{
	unsigned short attrib = (console_backcolour << 4) | (console_forecolour & 0x0F);
	unsigned short *vidmem = (unsigned short*)0xB8000 + (console_w * console_h);
	while(vidmem >= (unsigned short *)0xB8000)
	{
		*(vidmem--) = (attrib << 8);
	}
	cursor_x = cursor_y = 0;
	move_cursor();
}

// updates the hardware cursor.
void move_cursor()
{
	// The screen is 80 characters wide...
	unsigned short cursorLocation = cursor_y * console_w + cursor_x;
	outportb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outportb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outportb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outportb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// scroll down screen
void scroll()
{
	unsigned short attrib = (console_backcolour << 4) | (console_forecolour & 0x0F);
	unsigned short *vidmem = (unsigned short*)0xB8000;
	unsigned short *max = (unsigned short*)0xB8000 + (console_w * (console_h - 1));
	while(vidmem < max)
	{
		*(vidmem) = *(vidmem + console_w);
		vidmem++;
	}
	// clear new line
	max = (unsigned short*)0xB8000 + (console_w * console_h);
	while(vidmem < max)
	{
		*(vidmem++) = (attrib << 8);
	}
	cursor_y--;
}

// prints a string on the screen
void puts(char *message)
{
	char *vidmem = (char*)0xB8000 + (cursor_y * console_w + cursor_x) * 2;
	while(*message)
	{
		if (*message == '\n')
		{
			vidmem += (console_w - cursor_x) * 2;
			cursor_y++; cursor_x = 0;
		}
		else
		{
			*vidmem = *message;
			vidmem += 2;
			cursor_x++;
			if (cursor_x == console_w) { cursor_x = 0; cursor_y++; }
		}
		if (cursor_y == console_h) scroll();
		message++;
	}
	move_cursor();
}

char hex2string_buffer[10];
// converts number into hex format string
char* hex2string(int val)
{
	int i;
	int b, sh=7;
	hex2string_buffer[0] = '0';
	hex2string_buffer[1] = 'x';
	for (i=2; i<10; i++) 
	{
		b = val >> (sh-- * 4) & 0xF;
		hex2string_buffer[i] = (b < 10 ? 48 : 55) + b;
	}
	hex2string_buffer[10] = 0;
	return hex2string_buffer;
}

char bin2string_buffer[32];
// converts number into binary format string
char* bin2string(int val)
{
	int i;
	int b, sh=31;
	for (i=0; i<32; i++) 
	{
		b = val >> (sh--) & 0x1;
		bin2string_buffer[i] = 48 + b;
	}
	bin2string_buffer[32] = 0;
	return bin2string_buffer;
}

char dec2string_buffer[32];
// converts number into decimal format string
char* dec2string(int val)
{
	int i = 1, b;
	char s[32];
	s[0] = 0;
	while(val)
	{
		b = val % 10;
		s[i++] = 48 + b;
		val /= 10;
	}
	b = 0;
	// reverse the string and copy to buffer
	while((dec2string_buffer[b++] = s[--i]) && b < 32);
	return dec2string_buffer;
}
