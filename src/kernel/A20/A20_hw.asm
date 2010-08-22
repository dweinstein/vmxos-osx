global _enable_A20
_enable_A20:
	cli
	pusha
    call wait_input
    mov al, 0xAD
    out 0x64, al		; disable keyboard
    call wait_input

    mov al,0xD0
    out 0x64, al		; tell controller to read output port
    call wait_output

    in al, 0x60
    push eax			; get output port data and store it
    call wait_input

    mov al,0xD1
    out 0x64, al		; tell controller to write output port
    call wait_input

    pop eax
    or al, 2			; set bit 1 (enable a20)
    out 0x60,al			; write out data back to the output port

    call wait_input
    mov al, 0xAE		; enable keyboard
    out 0x64, al

    call wait_input
	popa
    ; sti				;; NOTE: enable this only when interrupt is handled, else will lead in triple fault
    ret

global _check_A20
_check_A20:
	cli
	push ebx
	push ecx
	push edx

    call wait_input

    mov al,0xAD
    out 0x64,al			; disable keyboard
    call wait_input

    mov al, 0xD0
    out 0x64, al		; tell controller to read output port
    call wait_output

    in al, 0x60
	shr eax, 1
	and eax, 1
    push eax			; get output port data and store it

    call wait_input
    mov al, 0xAE		; enable keyboard
    out 0x64, al

    call wait_input
	
	pop eax
	pop edx
	pop ecx
	pop ebx
    ; sti				;; NOTE: enable this only when interrupt is handled, else will lead in triple fault
    ret

; wait for input buffer to be clear
wait_input:
    in al, 0x64
    test al, 2
    jnz wait_input
    ret

; wait for output buffer to be clear
wait_output:
    in al, 0x64
    test al,1
    jz wait_output
    ret
