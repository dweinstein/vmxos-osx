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

	break_point();
	sgdt();
	puts("GDTR base : "); puts(hex2string(gdtr.base)); puts("\n");
	puts("GDTR limit: "); puts(hex2string(gdtr.limit)); puts("\n");
	//sidt();
	puts("IDTR base : "); puts(hex2string(idtr.base)); puts("\n");
	puts("IDTR limit: "); puts(hex2string(idtr.limit)); puts("\n");
	puts("IDT vector: "); puts(hex2string(&idt)); puts("\n");

	unsigned char u8 = 0;
	unsigned short u16 = 0;
	unsigned int u32 = 0;
	unsigned long long u64 = 0;

	// 16-Bit Host-State Fields
	asm("mov %%es, %0" : "=m" (u16));	vmx_vmwrite(Host_ES_selector, u16);
	asm("mov %%cs, %0" : "=m" (u16));	vmx_vmwrite(Host_CS_selector, u16);
	asm("mov %%ss, %0" : "=m" (u16));	vmx_vmwrite(Host_SS_selector, 0x18);
	asm("mov %%ds, %0" : "=m" (u16));	vmx_vmwrite(Host_DS_selector, u16);
	asm("mov %%fs, %0" : "=m" (u16));	vmx_vmwrite(Host_FS_selector, u16);
	asm("mov %%gs, %0" : "=m" (u16));	vmx_vmwrite(Host_GS_selector, u16);
	/*asm("str %0" : "=m" (u16));	*/	vmx_vmwrite(Host_TR_selector, 0x18);

	// 64-Bit Guest-State Fields
	vmx_vmwrite(VMCS_link_pointer_full, 0xFFFFFFFF);
	vmx_vmwrite(VMCS_link_pointer_high, 0xFFFFFFFF);

	vmx_read_msr(IA32_DEBUGCTL, &u64);
	vmx_vmwrite(Guest_IA32_DEBUGCTL_full, u64);
	vmx_vmwrite(Guest_IA32_DEBUGCTL_high, u64>>32);
	puts("Guest_IA32_DEBUGCTL_full: "); puts(hex2string(u64)); puts("\n");
	puts("Guest_IA32_DEBUGCTL_high: "); puts(hex2string(u64>>32)); puts("\n");

	// 32-Bit Control Fields
	vmx_vmwrite(Pin_based_VM_execution_controls, 0x1f);
	vmx_vmwrite(Processor_based_VM_execution_controls, 0x0401E9F2);
	vmx_vmwrite(VM_exit_controls, 0x36dff);
	vmx_vmwrite(VM_entry_controls, 0x11ff);
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

	// 16-Bit Guest-State Fields
	asm("mov %%es, %0" : "=m" (u16));	vmx_vmwrite(Guest_ES_selector, u16);
	asm("mov %%cs, %0" : "=m" (u16));	vmx_vmwrite(Guest_CS_selector, u16);
	asm("mov %%ss, %0" : "=m" (u16));	vmx_vmwrite(Guest_SS_selector, 0x18);
	asm("mov %%ds, %0" : "=m" (u16));	vmx_vmwrite(Guest_DS_selector, u16);
	asm("mov %%fs, %0" : "=m" (u16));	vmx_vmwrite(Guest_FS_selector, u16);
	asm("mov %%gs, %0" : "=m" (u16));	vmx_vmwrite(Guest_GS_selector, u16);
	asm("sldt %0" : "=m" (u16));		vmx_vmwrite(Guest_LDTR_selector, u16);
	/*asm("str %0" : "=m" (u16));*/		vmx_vmwrite(Guest_TR_selector, 0x18);

	// 32-Bit Guest-State Fields

	// NB: 3A 2.4.4 On power up or reset of the processor, the base address is set to the 
	// default value of 0 and the limit is set to 0FFFFH.
	vmx_vmwrite(Guest_CS_limit, 0xffffffff);
	vmx_vmwrite(Guest_ES_limit, 0xffffffff);
	vmx_vmwrite(Guest_SS_limit, 0xffffffff);
	vmx_vmwrite(Guest_DS_limit, 0xffffffff);
	vmx_vmwrite(Guest_FS_limit, 0xffffffff);
	vmx_vmwrite(Guest_GS_limit, 0xffffffff);
	vmx_vmwrite(Guest_LDTR_limit, 0xFFFF);
	vmx_vmwrite(Guest_TR_limit, 0xFF);

	vmx_vmwrite(Guest_GDTR_limit, gdtr.limit);
	vmx_vmwrite(Guest_IDTR_limit, idtr.limit);

	vmx_vmwrite(Guest_DR7, 0x400);

	vmx_vmwrite(Guest_interruptibility_state, 0);
	vmx_vmwrite(Guest_activity_state, 0);

	vmx_vmwrite(Guest_CS_access_rights, 0xC09B);
	vmx_vmwrite(Guest_ES_access_rights, 0xC093);
	vmx_vmwrite(Guest_SS_access_rights, 0xC093);
	vmx_vmwrite(Guest_DS_access_rights, 0xC093);
	vmx_vmwrite(Guest_FS_access_rights, 0xC093);
	vmx_vmwrite(Guest_GS_access_rights, 0xC093);
	vmx_vmwrite(Guest_LDTR_access_rights, 0x00010000);
	vmx_vmwrite(Guest_TR_access_rights, 0x8b);

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

	vmx_vmwrite(Guest_RSP, 0x90000);
	vmx_vmwrite(Guest_RIP, guest_entry);

	vmx_vmwrite(Host_RSP, 0x90000);
	vmx_vmwrite(Host_RIP, host_entry);

	if (!vmx_vmlaunch())
	{
		puts("IMPOSSIBILE LANCIARE LA VM\nError #"); 
		puts(vmx_instruction_errors[vmx_vmread(VM_instruction_error)]); 
		puts("\n");
	}

	puts("Leaving VMX mode "); puts(vmx_vmxoff() ? "success\n" : "failed\n");
	
	puts("Type something: ");

	while(1);
}

void guest_entry()
{
	puts("VM_ENTER: inside guest\n");
	hlt();
}

void host_entry()
{
	VM_EXIT_REASON_t er;
	UINT(er) = vmx_vmread(Exit_reason);
	puts("VM_EXIT: back to the host.. finally!\n");
	puts("Exit reason:\n");
	puts("- Basic exit reason: "); puts(vmx_exit_reasons[er.b0015_Basic_exit_reason]); puts("\n");
	puts("- Pending MTF VM exit: "); puts(dec2string(er.b28_Pending_MTF_VM_exit)); puts("\n");
	puts("- VM exit from VMX root operation: "); puts(dec2string(er.b29_VM_exit_from_VMX_root_operation)); puts("\n");
	puts("- VM-entry failure: "); puts(dec2string(er.b31_VM_entry_failure)); puts("\n");
	sti();
	//hlt();
	while(1);
}
