#include "main.h"

void main()
{
	CPUID_t cpu;
	CR4_t cr4;
	CR0_t cr0;
	EFLAGS_t eflags;
	unsigned int a20 = check_A20();

	cls();
	puts("Welcome to VMOS\n");
	puts("Inizialising the kernel modules\n\n");

	puts("A20   : "); puts(a20 ? "enabled\n" : "not enabled\n");
	if (!a20) { puts("enabling A20 gate "); enable_A20(); puts("[done]\n"); }

	UINT(cpu) = cpuid();
	puts("CPUID : "); puts(hex2string(UINT(cpu))); puts(" = "); puts(bin2string(UINT(cpu))); puts("\n");
	puts("VMX   : "); puts(cpu.b05_VMX ? "supported\n" : "not supported\n");

	UINT(eflags) = vmx_read_eflags();
	puts("EFLAGS: "); puts(hex2string(UINT(eflags))); puts(" = "); puts(bin2string(UINT(eflags))); puts("\n");
	puts("VM86 Mode is "); puts(eflags.b17_VM ? "on\n" : "off\n");

	UINT(cr4) = vmx_read_cr4();
	cr4.b13_VMXE = 1;
	puts("enabling CR4.VME "); vmx_write_cr4(UINT(cr4)); puts("[done]\n");

	UINT(cr0) = vmx_read_cr0();
	cr0.b00_PE = 1; puts("enabling CR0.PE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	cr0.b05_NE = 1; puts("enabling CR0.NE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	puts("enabling paging "); enable_paging(); puts("[done]\n");
	
	puts("cr0   : "); puts(hex2string(vmx_read_cr0())); puts(" = "); puts(bin2string(vmx_read_cr0())); puts("\n");
	puts("cr3   : "); puts(hex2string(vmx_read_cr3())); puts(" = "); puts(bin2string(vmx_read_cr3())); puts("\n");
	puts("cr4   : "); puts(hex2string(vmx_read_cr4())); puts(" = "); puts(bin2string(vmx_read_cr4())); puts("\n");

	remap_pic(0x20, 0x28);
	mask_irq(IRQ_TIMER);
	//unmask_irq(IRQ_KEYBOARD);
	puts("Inizializing interrupt handlers "); init_exceptions(); puts("[done]\n");
	sti();
	//invoke48();

	puts("\n\nType something: ");

	while(1);
}
