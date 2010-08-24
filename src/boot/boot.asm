bits 16
org 0x7C00

%macro print 1
	mov si, %1
	call _k_print
%endmacro

%macro cls 0
	mov ax, 0003h
	int 10h
%endmacro

mov	[drive], dl			; Store boot disk

cli						; clear interrupts

;mov ax, 0x07C0			; setup registers to point to our segment
;mov ds, ax
;mov es, ax
;mov fs, ax
;mov gs, ax

;mov ax, 0x0000			; set the stack
;mov ss, ax
;mov sp, 0xFFFF

; hide the cursor
;mov ah, 1
;mov ch, 0x20
;int 0x10

cls
print msg

reset:
	mov		ah, 0		; reset floppy disk function
	mov		dl, [drive]	; drive 0 is floppy drive
	int		0x13		; call BIOS
	jc		reset		; If Carry Flag (CF) is set, there was an error. Try resetting again

	mov		ax, 0		; we are going to read sector to into address 0x7E00:0
	mov		es, ax
	mov		bx, 0x1000
read:
	mov		ah, 0x02	; function 2
	mov		al, 30		; read 1 sector
	mov		ch, 0		; we are reading the second sector past us, so its still on track 1
	mov		cl, 2		; sector to read (The second sector)
	mov		dh, 0		; head number
	mov		dl, [drive]	; drive number. Remember Drive 0 is floppy drive.
	int		0x13		; call BIOS - Read the sector
	jc		read		; Error, so try again

lgdt [toc]				; load GDT into GDTR
mov	eax, cr0			; set bit 0 in cr0--enter pmode
or	eax, 1
mov	cr0, eax

jmp	08h:pmode			; far jump to fix CS. Remember that the code selector is 0x8!

bits 32					; Welcome to the 32 bit world!
pmode:
	mov	ax, 0x10		; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h
	
	jmp 08h:0x1000

gdt_data: 
	dd 0 				; null descriptor
	dd 0 
; gdt code:				; code descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 		; access
	db 11001111b 		; granularity
	db 0 				; base high
; gdt data:				; data descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 		; access
	db 11001111b 		; granularity
	db 0				; base high
toc: 
	dw $ - gdt_data - 1 ; limit (Size of GDT)
	dd gdt_data 		; base of GDT

drive db 0
msg db "booting vmxos...", 13, 10, 0

_k_print:
	mov ah, 0Eh			; We want to print a single character
	mov bh, 0Eh			; White text on black background, not blinking
	mov bl, 0			; Page number 0
.chr:
	lodsb
	cmp al, 0
	je .done
	int 10h
	jmp .chr
.done:
	ret

times 510-($-$$) db 0
dw 0xAA55