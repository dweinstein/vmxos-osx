bits 32

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