bits 32
jmp start

global _break_point
_break_point:
	ret

extern _main
global start
start:
	call _main
	cli
	hlt

global _hlt
_hlt:
	hlt
	ret