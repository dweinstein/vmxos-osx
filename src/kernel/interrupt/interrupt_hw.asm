global _lidt
_lidt:
	mov eax, [esp+4]
	lidt [eax]
	ret

global _cli
_cli:
	cli
	ret

global _sti
_sti:
	sti
	ret

global _invoke48
_invoke48:
	int 48
	ret

extern _exception_handler
extern _hwint_handler
extern _syscall_handler

global _int48
_int48:
	push eax
	call _syscall_handler
	pop eax
	iret

global _hwint
_hwint:
	push eax
	call _hwint_handler
	pop eax
	iret

%macro exception_handler 1
	global _int%1
	_int%1:
		 mov eax, %1
		 push eax
		 call _exception_handler
		 pop eax
		 iret
%endmacro

exception_handler  0	; "#00 Divide By Zero Error",
exception_handler  1	; "#DB Debug Error",
exception_handler  2	; "#-- NMI Interrupt",
exception_handler  3	; "#BP Breakpoint",
exception_handler  4	; "#OF Overflow",
exception_handler  5	; "#BR BOUND Range Exceeded"
exception_handler  6	; "#UD Invalid Opcode",
exception_handler  7	; "#NM Device Not Available",
exception_handler  8	; "#DF Double Fault",
exception_handler  9	; "#-- Coprocessor Segment Overrun",
exception_handler  10	; "#TS Invalid TSS",
exception_handler  11	; "#NP Segment Not Present",
exception_handler  12	; "#SS Stack Segment Fault",
exception_handler  13	; "#GP Gneral Protection Fault",
exception_handler  14	; "#PF Page Fault",
exception_handler  15	; "#15 reserved",
exception_handler  16	; "#MF FPU Floating-Point Exception",
exception_handler  17	; "#AC Alignment Check",
exception_handler  18	; "#MC Machine Check",
exception_handler  19	; "#XF SIMD Floating-Point Exception",
exception_handler  20	; "#20 reserved",
exception_handler  21	; "#21 reserved",
exception_handler  22	; "#22 reserved",
exception_handler  23	; "#23 reserved",
exception_handler  24	; "#24 reserved",
exception_handler  25	; "#25 reserved",
exception_handler  26	; "#26 reserved",
exception_handler  27	; "#27 reserved",
exception_handler  28	; "#28 reserved",
exception_handler  29	; "#29 reserved",
exception_handler  30	; "#SX Security Exception",
exception_handler  31	; "#31 reserved",
