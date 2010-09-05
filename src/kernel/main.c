#include "main.h"

void guest_entry();
void host_entry();

void main()
{
	CPUID_t cpu;
	CR4_t cr4;
	CR0_t cr0;
	EFLAGS_t eflags;
	unsigned int a20;

	cls();
	puts("Welcome to VMXOS "); puts(vmxos_version); puts(" build "); puts(dec2string(vmxos_build)); puts("\n");
	puts("Inizialising the kernel modules\n\n");
	
	disable_A20();
	a20 = check_A20();
	puts("A20   : "); puts(a20 ? "enabled\n" : "not enabled -> ");
	if (!a20) 
	{ 
		puts("enabling A20 gate... "); 
		enable_A20();
		if (check_A20())
		{
			puts("successed\n"); 
		}
		else 
		{
			puts("failed\n"); 
			return;
		}
	}

	cpuid(&cpu);
	puts("CPUID : EAX -> "); puts(hex2string(cpu.eax)); puts(" = "); puts(bin2string(cpu.eax)); puts("\n");
	puts("      : EBX -> "); puts(hex2string(cpu.ebx)); puts(" = "); puts(bin2string(cpu.ebx)); puts("\n");
	puts("      : ECX -> "); puts(hex2string(UINT(cpu.ecx))); puts(" = "); puts(bin2string(UINT(cpu.ecx))); puts("\n");
	puts("      : EDX -> "); puts(hex2string(UINT(cpu.edx))); puts(" = "); puts(bin2string(UINT(cpu.edx))); puts("\n");
	puts("VMX   : "); puts(cpu.ecx.b05_VMX ? "supported\n" : "not supported\n");
	puts("MSR RW: "); puts(cpu.edx.b05_MSR ? "supported\n" : "not supported\n");
	if (!cpu.ecx.b05_VMX || !cpu.edx.b05_MSR) return;

	UINT(eflags) = vmx_read_eflags();
	puts("EFLAGS: "); puts(hex2string(UINT(eflags))); puts(" = "); puts(bin2string(UINT(eflags))); puts("\n");
	puts("VM86 Mode is "); puts(eflags.b17_VM ? "on\n" : "off\n");

	UINT(cr4) = vmx_read_cr4();
	cr4.b13_VMXE = 1;
	puts("Enabling CR4.VME "); vmx_write_cr4(UINT(cr4)); puts("[done]\n");

	UINT(cr0) = vmx_read_cr0();
	cr0.b00_PE = 1; puts("Enabling CR0.PE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	cr0.b05_NE = 1; puts("Enabling CR0.NE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	puts("Enabling paging "); enable_paging(); puts("[done]\n");
	
	puts("CR0   : "); puts(hex2string(vmx_read_cr0())); puts(" = "); puts(bin2string(vmx_read_cr0())); puts("\n");
	puts("CR2   : "); puts(hex2string(vmx_read_cr2())); puts(" = "); puts(bin2string(vmx_read_cr2())); puts("\n");
	puts("CR3   : "); puts(hex2string(vmx_read_cr3())); puts(" = "); puts(bin2string(vmx_read_cr3())); puts("\n");
	puts("CR4   : "); puts(hex2string(vmx_read_cr4())); puts(" = "); puts(bin2string(vmx_read_cr4())); puts("\n");

	remap_pic(0x20, 0x28);
	mask_irq(IRQ_TIMER);
	//unmask_irq(IRQ_KEYBOARD);
	puts("Inizializing interrupt handlers "); init_exceptions(); puts("[done]\n");
	sti();
	
	//invoke48();
	
	unsigned long long nl = 0;
	puts("Reading MSR 0xA3");
	vmx_read_msr(0x3A, &nl);
	puts(" -> "); puts(bin2string(nl)); puts("\n");
	if (!(nl & 1))
	{
		puts("Locking MSR...\n");
		nl |= 1; // set bit 0
		vmx_write_msr(0x3A, &nl);
		puts("MSR 0x3A locked -> "); puts(bin2string(nl)); puts("\n");
	}
	else
	{
		puts("MSR 0x3A already locked\n");
	}

	vmx_read_msr(0x480, &nl);
	vmxon_ptr = 0x9E000;
	vmcs_ptr = 0x9F000;

	int i;
	unsigned int* reg;
	
	reg = (unsigned int*)vmxon_ptr;
	for(i = 0; i < 1024; i++) reg[i] = 0;
	reg[0] = nl;
	
	reg = (unsigned int*)vmcs_ptr;
	for(i = 0; i < 1024; i++) reg[i] = 0;
	reg[0] = nl;

	puts("VMX revID: "); puts(hex2string(*reg));
	puts(" - "); puts("vmx region at: "); puts(hex2string(vmxon_ptr));
	puts(" - "); puts("vmcs region at: "); puts(hex2string(vmcs_ptr)); puts("\n");

	puts("Entering VMX mode "); puts(vmx_vmxon() ? "success\n" : "failed\n");
	puts("Clearing VMCS region "); puts(vmx_vmclear() ? "success\n" : "failed\n");
	puts("Loading VMCS pointer "); puts(vmx_vmptrld() ? "success\n" : "failed\n");

	sgdt();
	puts("GDTR base: "); puts(hex2string(gdtr.base)); puts("\n");
	puts("GDTR limit: "); puts(hex2string(gdtr.limit)); puts("\n");

	unsigned char u8 = 0;
	unsigned short u16 = 0;
	unsigned int u32 = 0;
	unsigned long long u64 = 0;

//	// Initialize VMX Controls:
//	vmx_vmwrite(VM_entry_controls, 0x11FF);
//	vmx_vmwrite(Pin_based_VM_execution_controls, 0x1F);
//	vmx_vmwrite(Processor_based_VM_execution_controls, 0x0401E9F2);
//	vmx_vmwrite(VM_exit_controls, 0x36dff);
//
//// initialize_vmx_host_guest_state
//asm("mov %%cs, %0" : "=m" (selector));
//vmx_vmwrite(Host_CS_selector, selector);
//asm("mov %%ds, %0" : "=m" (selector));
//vmx_vmwrite(Host_DS_selector, selector);
//vmx_vmwrite(Host_SS_selector, 0x18);
//vmx_vmwrite(Host_TR_selector, 0x18);
//vmx_vmwrite(Guest_TR_selector, 0x18);
//vmx_vmwrite(Guest_TR_access_rights, 0x8B);
//vmx_vmwrite(Guest_TR_limit, 0xFF);
//vmx_vmwrite(Guest_LDTR_access_rights, 0x10000);
//vmx_vmwrite(Guest_SS_access_rights, 0xC093);
//vmx_vmwrite(Guest_DS_access_rights, 0xC093);
//vmx_vmwrite(Guest_ES_access_rights, 0xC093);
//vmx_vmwrite(Guest_FS_access_rights, 0xC093);
//vmx_vmwrite(Guest_GS_access_rights, 0xC093);
//vmx_vmwrite(Guest_SS_limit, 0xFFFFFFFF);
//vmx_vmwrite(Guest_DS_limit, 0xFFFFFFFF);
//vmx_vmwrite(Guest_ES_limit, 0xFFFFFFFF);
//vmx_vmwrite(Guest_FS_limit, 0xFFFFFFFF);
//vmx_vmwrite(Guest_GS_limit, 0xFFFFFFFF);
//vmx_vmwrite(VMCS_link_pointer_full, 0xFFFFFFFF);
//mov ebx, VMS_LINK_PTR_HIGH ;0x00002801
//vmwrite ebx, eax
//vmx_vmwrite(Guest_DS_limit, 0xFFFFFFFF);
//
//mov ebx, GUEST_GDTR_BASE ;0x00006816
//mov eax, gdt32t
//vmwrite ebx, eax
//mov ebx, HOST_GDTR_BASE ;0x00006c0c
//vmwrite ebx, eax
//ov ebx, GUEST_CS_LIMIT ;0x00004802
//mov eax, 0xffffffff
//vmwrite ebx, eax
//mov ebx, GUEST_CS_ATTR ;0x00004816
//mov eax, 0xc09b
//vmwrite ebx, eax
//mov ebx, GUEST_RSP ;0x0000681c
//mov eax, tos
//vmwrite ebx, eax
//mov ebx, GUEST_IDTR_BASE ;0x00006818
//mov eax, idt32t
//vmwrite ebx, eax
//mov ebx, HOST_IDTR_BASE ;0x00006c0e
//vmwrite ebx, eax
//mov ebx, GUEST_CS_SEL ;0x00000802
//mov eax, guest_sel
//vmwrite ebx, eax
//mov ebx, GUEST_CS_BASE ;0x00006808
//mov eax, guest_base
//vmwrite ebx, eax
//mov ebx, GUEST_RIP ;0x0000681e
//mov eax, 0
//vmwrite ebx, eax
//mov ebx, HOST_RIP ;0x00006c16
//mov eax, after_vmexit
//vmwrite ebx, eax
//mov ebx, GUEST_RFLAGS ;0x00006820
//mov eax, 2
//vmwrite ebx, eax
//mov ebx, EXCEPTION_BITMAP ;0x4004
//mov eax,0xdeadfeef
//vmwrite ebx, eax

// 16-Bit Guest Fields
asm("mov %%es, %0" : "=m" (u16));	vmx_vmwrite(Guest_ES_selector, u16);	puts("ES sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%cs, %0" : "=m" (u16));	vmx_vmwrite(Guest_CS_selector, u16);	puts("CS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%ss, %0" : "=m" (u16));	vmx_vmwrite(Guest_SS_selector, u16);	puts("SS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%ds, %0" : "=m" (u16));	vmx_vmwrite(Guest_DS_selector, u16);	puts("DS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%fs, %0" : "=m" (u16));	vmx_vmwrite(Guest_FS_selector, u16);	puts("FS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%gs, %0" : "=m" (u16));	vmx_vmwrite(Guest_GS_selector, u16);	puts("GS sel: "); puts(hex2string(u16)); puts("\n");
asm("sldt %0" : "=m" (u16));		vmx_vmwrite(Guest_LDTR_selector, u16);	puts("LDTR sel: "); puts(hex2string(u16)); puts("\n");
asm("str %0" : "=m" (u16));			vmx_vmwrite(Guest_TR_selector, u16);	puts("TR sel: "); puts(hex2string(u16)); puts("\n");

// 16-Bit Host Fields
asm("mov %%es, %0" : "=m" (u16));	vmx_vmwrite(Host_ES_selector, u16);		puts("ES sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%cs, %0" : "=m" (u16));	vmx_vmwrite(Host_CS_selector, u16);		puts("CS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%ss, %0" : "=m" (u16));	vmx_vmwrite(Host_SS_selector, u16);		puts("SS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%ds, %0" : "=m" (u16));	vmx_vmwrite(Host_DS_selector, u16);		puts("DS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%fs, %0" : "=m" (u16));	vmx_vmwrite(Host_FS_selector, u16);		puts("FS sel: "); puts(hex2string(u16)); puts("\n");
asm("mov %%gs, %0" : "=m" (u16));	vmx_vmwrite(Host_GS_selector, u16);		puts("GS sel: "); puts(hex2string(u16)); puts("\n");
asm("str %0" : "=m" (u16));			vmx_vmwrite(Host_TR_selector, u16);		puts("LDTR sel: "); puts(hex2string(u16)); puts("\n");

// 64-Bit Guest-State Fields
vmx_vmwrite(VMCS_link_pointer_full, 0xFFFFFFFF);
vmx_vmwrite(VMCS_link_pointer_high, 0xFFFFFFFF);

vmx_read_msr(IA32_DEBUGCTL, &u64);
vmx_vmwrite(Guest_IA32_DEBUGCTL_full, u64);
vmx_vmwrite(Guest_IA32_DEBUGCTL_high, u64>>32);
puts("Guest_IA32_DEBUGCTL_full: "); puts(hex2string(u64)); puts("\n");
puts("Guest_IA32_DEBUGCTL_high: "); puts(hex2string(u64>>32)); puts("\n");

// 32-Bit Control Fields
vmx_vmwrite(Pin_based_VM_execution_controls, 0);
vmx_vmwrite(Processor_based_VM_execution_controls, 0x2000080);
vmx_vmwrite(VM_exit_controls, 1<<15);
vmx_vmwrite(VM_entry_controls, 0);
vmx_vmwrite(VM_exit_MSR_store_count, 0);
vmx_vmwrite(VM_exit_MSR_load_count, 0);
vmx_vmwrite(VM_entry_MSR_load_count, 0);
vmx_vmwrite(VM_entry_interruption_information_field, 0);

vmx_vmwrite(TSC_offset_full, 0);
vmx_vmwrite(TSC_offset_high, 0);
vmx_vmwrite(Page_fault_error_code_mask, 0);
vmx_vmwrite(Page_fault_error_code_match, 0);
vmx_vmwrite(CR3_target_count, 0);
vmx_vmwrite(CR3_target_value_0, 0);
vmx_vmwrite(CR3_target_value_1, 0);
vmx_vmwrite(CR3_target_value_2, 0);
vmx_vmwrite(CR3_target_value_3, 0);

// 32-Bit Guest-State Fields

// NB: 3A 2.4.4 On power up or reset of the processor, the base address is set to the 
// default value of 0 and the limit is set to 0FFFFH.
vmx_vmwrite(Guest_CS_limit, 0xFFFF);
vmx_vmwrite(Guest_ES_limit, 0xFFFF);
vmx_vmwrite(Guest_SS_limit, 0xFFFF);
vmx_vmwrite(Guest_DS_limit, 0xFFFF);
vmx_vmwrite(Guest_FS_limit, 0xFFFF);
vmx_vmwrite(Guest_GS_limit, 0xFFFF);
vmx_vmwrite(Guest_LDTR_limit, 0xFFFF);
vmx_vmwrite(Guest_TR_limit, 0xFFFF);

vmx_vmwrite(Guest_GDTR_limit, gdtr.limit);
vmx_vmwrite(Guest_IDTR_limit, idtr.limit);

vmx_vmwrite(Guest_DR7, 0x400);

vmx_vmwrite(Guest_interruptibility_state, 0);
vmx_vmwrite(Guest_activity_state, 0);

vmx_vmwrite(Guest_CS_access_rights, 0x9A);
vmx_vmwrite(Guest_ES_access_rights, 0x92);
vmx_vmwrite(Guest_SS_access_rights, 0x92);
vmx_vmwrite(Guest_DS_access_rights, 0x92);
vmx_vmwrite(Guest_FS_access_rights, 0x92);
vmx_vmwrite(Guest_GS_access_rights, 0x92);
vmx_vmwrite(Guest_LDTR_access_rights, 0);
vmx_vmwrite(Guest_TR_access_rights, 0);

// Natural-Width Guest-State Fields
vmx_read_msr(IA32_SYSENTER_CS, &u64);
vmx_vmwrite(Guest_IA32_SYSENTER_CS, u64);

vmx_vmwrite(Guest_CR0, vmx_read_cr0());
vmx_vmwrite(Guest_CR3, vmx_read_cr3());
vmx_vmwrite(Guest_CR4, vmx_read_cr4());

vmx_vmwrite(Guest_ES_base, 0);
vmx_vmwrite(Guest_CS_base, 0);
vmx_vmwrite(Guest_SS_base, 0);
vmx_vmwrite(Guest_DS_base, 0);
vmx_vmwrite(Guest_FS_base, 0);
vmx_vmwrite(Guest_GS_base, 0);
vmx_vmwrite(Guest_LDTR_base, 0);
vmx_vmwrite(Guest_TR_base, 0);

vmx_vmwrite(Guest_GDTR_base, gdtr.base);
vmx_vmwrite(Guest_IDTR_base, idtr.base);

UINT(eflags) = vmx_read_eflags();
vmx_vmwrite(Guest_RFLAGS, UINT(eflags));

vmx_read_msr(IA32_SYSENTER_ESP, &u64);
vmx_vmwrite(Guest_IA32_SYSENTER_ESP, u64);
vmx_read_msr(IA32_SYSENTER_EIP, &u64);
vmx_vmwrite(Guest_IA32_SYSENTER_EIP, u64);


// Natural-Width Host-State Fields
vmx_vmwrite(Host_CR0, vmx_read_cr0());
vmx_vmwrite(Host_CR3, vmx_read_cr3());
vmx_vmwrite(Host_CR4, vmx_read_cr4());

vmx_vmwrite(Host_FS_base, 0);
vmx_vmwrite(Host_GS_base, 0);
vmx_vmwrite(Host_TR_base, 0);
vmx_vmwrite(Host_GDTR_base, gdtr.base);
vmx_vmwrite(Host_IDTR_base, idtr.base);

vmx_read_msr(IA32_SYSENTER_ESP, &u64);
vmx_vmwrite(Host_IA32_SYSENTER_ESP, u64);
vmx_read_msr(IA32_SYSENTER_EIP, &u64);
vmx_vmwrite(Host_IA32_SYSENTER_EIP, u64);
vmx_read_msr(IA32_SYSENTER_CS, &u64);
vmx_vmwrite(Host_IA32_SYSENTER_CS, u64);

vmx_vmwrite(Guest_RSP, 0x80000);
vmx_vmwrite(Guest_RIP, guest_entry);

vmx_vmwrite(Host_RSP, 0x90000);
vmx_vmwrite(Host_RIP, host_entry);

if (!vmx_vmlaunch()) puts("IMPOSSIBILE LANCIARE LA VM\n");

	puts("Leaving VMX mode "); puts(vmx_vmxoff() ? "success\n" : "failed\n");
	
	puts("Type something: ");

	while(1);
}

void guest_entry()
{
	puts("inside guest");
	hlt();
}

void host_entry()
{
	puts("back to the host");
	sti();
	hlt();
}
