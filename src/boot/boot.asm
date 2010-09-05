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

xor ax, ax				; setup registers to point to our segment
mov ds, ax

mov	[drive], dl			; Store boot disk

cmp dl, 0x80
je from_hdd

print msg_floppy
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
	
	jmp set_pmode

; GRUB STAGE1
; 0- 2	0x0010 packet e reserved
; 2- 4	0x0001 n blocks
; 4- 6	0x0000 buffer offset
; 6- 8	0x7000 buffer segment
; 8-12	0x0000.0001 
;12-16	0x0000.0000

; GRUB FEATHER
; 0- 2	0x0010 packet e reserved
; 2- 4	0x0001 n blocks
; 4- 6	0x7E00 buffer offset
; 6- 8	0x0000 buffer segment
; 8-12	0x0000.8034 
;12-16	0x0000.0000

;offset for CRUISER 0x2C
;0x2C rappresenta il numero di settori nascosti (nel BPB offset 0x1C 4byte)

DAPACK:
		db	0x10		; size of packet (16 bytes)
		db	0			; always 0
blkcnt	dw	30			; int 13 resets this to # of blocks actually read/written
db_add	dw	0x1000		; memory buffer destination address (0:7c00)
		dw	0			; in memory page zero
d_lba	dd	0x2d		; put the lba to read in this spot
		dd	0			; more storage bytes only for big lba's ( > 4 bytes )
 
from_hdd:

	; test if extension supported
	mov ah, 0x41
	mov bx, 0x55AA
	mov dl, [drive]
	int 0x13
	jb short no_hdsupport
	jc short no_hdsupport
	cmp bx, 0xAA55
	jne no_hdsupport
	test cl, 1
	jz no_hdsupport
		jmp read_hd
	no_hdsupport:
		print msg_hd_no
		jmp $


read_hd:
	print msg_hd

	mov si, DAPACK		; address of "disk address packet"
	mov ah, 0x42		; AL is unused
	mov dl, [drive]		; drive number 0 (OR the drive # with 0x80)
	int 0x13
	jc short .error
	cmp al, 0
	jne .error
	jmp set_pmode
.error:
	print msg_hd_err
	jmp $

set_pmode:

xor ax, ax				; setup registers to point to our segment
mov ds, ax

lgdt [toc]				; load GDT into GDTR

cli

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
	mov gs, ax
	mov es, ax
	mov	esp, 90000h		; stack begins from 90000h
	
	jmp 08h:0x1000

; GDT FORMAT
; dw 0FFFFh               ; Limit 0xFFFF
; dw 0                    ; Base 0:15
; db 0                    ; Base 16:23
; db 10011010b            ; Present, Ring 0, Code, Non-conforming, Readable
; db 11001111b            ; Page-granular
; db 0                    ; Base 24:31

bits 16
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

drive		db 0
msg_boot	db "booting vmxos...", 13, 10, 0
msg_floppy	db "boot from floppy", 13, 10, 0
msg_hd		db "boot from hd", 13, 10, 0
msg_hd_err	db "hd error", 13, 10, 0
msg_hd_no	db "hd lba ext not supported", 13, 10, 0
msg_hd_yes	db "hd lba ext supported", 13, 10, 0

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
