#ifndef _IO_H_
#define _IO_H_

inline void outportb(unsigned int port, unsigned char value);
inline void outportw(unsigned int port, unsigned int value);
inline unsigned char inportb(unsigned int port);

#endif