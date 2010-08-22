global _cpuid
_cpuid:
	push ebx
	push ecx
	push edx
	push edi

	mov eax, 1
	cpuid

	mov edi, [esp+20]
	mov dword [edi], eax
	add edi, 4
	mov dword [edi], ebx
	add edi, 4
	mov dword [edi], ecx
	add edi, 4
	mov dword [edi], edx

	pop edi
	pop edx
	pop ecx
	pop ebx
	ret

;; READ CONTROL REGISTERS

global _vmx_read_cr0
_vmx_read_cr0:
	mov eax, cr0
	ret

global _vmx_read_cr3
_vmx_read_cr3:
	mov eax, cr3
	ret

global _vmx_read_cr4
_vmx_read_cr4:
	mov eax, cr4
	ret

;; WRITE CONTROL REGISTERS

global _vmx_write_cr0
_vmx_write_cr0:
	mov eax, [esp+4]
	mov cr0, eax
	ret

global _vmx_write_cr3
_vmx_write_cr3:
	mov eax, [esp+4]
	mov cr3, eax
	ret

global _vmx_write_cr4
_vmx_write_cr4:
	mov eax, [esp+4]
	mov cr4, eax
	ret

global _vmx_read_eflags
_vmx_read_eflags:
	pushfd
	pop eax
	ret

global _vmx_write_eflags
_vmx_write_eflags:
	mov eax, [esp+4]
	push eax
	popfd
	ret
