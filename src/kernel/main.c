#include "main.h"

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

	puts("Entering vmx root mode "); puts(vmx_vmxon() ? "success\n" : "failed\n");
	puts("Clearing VMCS region "); puts(vmx_vmclear() ? "success\n" : "failed\n");
	puts("Loading VMCS pointer "); puts(vmx_vmptrld() ? "success\n" : "failed\n");
	
	puts("Type something: ");

	while(1);
}
