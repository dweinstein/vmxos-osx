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

extern _generic_int_handler
global _generic_int
_generic_int:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call _generic_int_handler
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

extern _generic_pic_handler
global _generic_pic
_generic_pic:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call _generic_pic_handler
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

extern _int_48
global _int48
_int48:
     pusha
     push ds
     push es
     push fs
     push gs
     mov eax,0x10    ; Data segment
     mov ds,eax
     mov es,eax
     cld
     call _int_48
     pop gs
     pop fs
     pop es
     pop ds
     popa
     iret

global _invoke48
_invoke48:
	int 48
	ret
