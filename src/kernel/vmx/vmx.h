#ifndef _VMX_H_
#define _VMX_H_

/************************
 * CONSTANTS DEFINITION
 ************************/
// If CPUID.01H:ECX.[bit 9]
#define IA32_VMX_BASIC				0x00000480		// BASE Register of VMX Capability Reporting. (R/O)
#define IA32_VMX_PINBASED_CTLS		0x00000481		// Capability Reporting Register of Pin-based VMCS Controls. (R/O)
#define IA32_VMX_PROCBASED_CTLS		0x00000482		// Capability Reporting Register of Processor-based VMCS Controls. (R/O)
#define IA32_VMX_EXIT_CTLS			0x00000483		// Capability Reporting Register of VM-exit VMCS Controls. (R/O)
#define IA32_VMX_ENTRY_CTLS			0x00000484		// Capability Reporting Register of VM-entry VMCS Controls. (R/O)
#define IA32_VMX_MISC  				0x00000485  	// Capability Reporting Register of Miscellaneous VMCS Controls. (R/O)
#define IA32_VMX_CR0_FIXED0  		0x00000486  	// Capability Reporting Register of CR0 Bits Fixed to Zero. (R/O)
#define IA32_VMX_CR0_FIXED1  		0x00000487  	// Capability Reporting Register of CR0 Bits Fixed to One. (R/O)
#define IA32_VMX_CR4_FIXED0  		0x00000488  	// Capability Reporting Register of CR4 Bits Fixed to Zero. (R/O)
#define IA32_VMX_CR4_FIXED1  		0x00000489  	// Capability Reporting Register of CR4 Bits Fixed to One. (R/O)
#define IA32_VMX_VMCS_ENUM  		0x0000048A  	// Capability Reporting Register of VMCS Field Enumeration. (R/O)

// If CPUID.80000001.EDX.[bit 20] or CPUID.80000001.EDX.[bit29]
#define IA32_EFER  					0xC0000080  	// Extended Feature Enables.
		// bit 0	  	SYSCALL Enable (R/W). Enables SYSCALL/SYSRET instructions in 64-bit mode.
		// bit 7:1  	Reserved.
		// bit 8		IA-32e Mode Enable. (R/W). Enables IA-32e mode operation.
		// bit 9		Reserved.
		// bit 10		IA-32e Mode Active. (R) Indicates IA-32e mode is active when set.
		// bit 11  		Execute Disable Bit Enable. (R/W) Enables the Execute-Disable-Bit functionality in paging structures.
		// bit 63:12  	Reserved.

// 16-Bit Guest Fields
#define Guest_ES_selector						0x00000800
#define Guest_CS_selector						0x00000802
#define Guest_SS_selector						0x00000804
#define Guest_DS_selector						0x00000806
#define Guest_FS_selector						0x00000808
#define Guest_GS_selector						0x0000080A
#define Guest_LDTR_selector						0x0000080C
#define Guest_TR_selector						0x0000080E

// 16-Bit Host Fields
#define Host_ES_selector						0x00000C00
#define Host_CS_selector						0x00000C02
#define Host_SS_selector						0x00000C04
#define Host_DS_selector						0x00000C06
#define Host_FS_selector						0x00000C08
#define Host_GS_selector						0x00000C0A
#define Host_TR_selector						0x00000C0C

// 64-Bit Control Fields
#define Address_of_IO_bitmap_A_full				0x00002000
#define Address_of_IO_bitmap_A_high				0x00002001
#define Address_of_IO_bitmap_B_full				0x00002002
#define Address_of_IO_bitmap_B_high				0x00002003
#define Address_of_MSR_bitmaps_full				0x00002004
#define Address_of_MSR_bitmaps_high				0x00002005
#define VM_exit_MSR_store_address_full			0x00002006
#define VM_exit_MSR_store_address_high			0x00002007
#define VM_exit_MSR_load_address_full			0x00002008
#define VM_exit_MSR_load_address_high			0x00002009
#define VM_entry_MSR_load_address_full			0x0000200A
#define VM_entry_MSR_load_address_high			0x0000200B
#define Executive_VMCS_pointer_full				0x0000200C
#define Executive_VMCS_pointer_high				0x0000200D
#define TSC_offset_full							0x00002010
#define TSC_offset_high							0x00002011
#define Virtual_APIC_page_address_full			0x00002012
#define Virtual_APIC_page_address_high			0x00002013

// 64-Bit Guest-State Fields
#define VMCS_link_pointer_full					0x00002800
#define VMCS_link_pointer_high					0x00002801
#define Guest_IA32_DEBUGCTL_full				0x00002802
#define Guest_IA32_DEBUGCTL_high				0x00002803

// 32-Bit Control Fields
#define Pin_based_VM_execution_controls			0x00004000
#define Processor_based_VM_execution_controls	0x00004002
#define Exception_bitmap						0x00004004
#define Page_fault_error_code_mask				0x00004006
#define Page_fault_error_code_match				0x00004008
#define CR3_target_count						0x0000400A
#define VM_exit_controls						0x0000400C
#define VM_exit_MSR_store_count					0x0000400E
#define VM_exit_MSR_load_count					0x00004010
#define VM_entry_controls						0x00004012
#define VM_entry_MSR_load_count					0x00004014
#define VM_entry_interruption_information_field	0x00004016
#define VM_entry_exception_error_code			0x00004018
#define VM_entry_instruction_length				0x0000401A
#define TPR_threshold							0x0000401C

// 32-Bit Read-Only Data Fields
#define VM_instruction_error					0x00004400
#define Exit_reason								0x00004402
#define VM_exit_interruption_information		0x00004404
#define VM_exit_interruption_error_code			0x00004406
#define IDT_vectoring_information_field			0x00004408
#define IDT_vectoring_error_code				0x0000440A
#define VM_exit_instruction_length				0x0000440C
#define VMX_instruction_information				0x0000440E

// 32-Bit Guest-State Fields
#define Guest_ES_limit							0x00004800
#define Guest_CS_limit							0x00004802
#define Guest_SS_limit							0x00004804
#define Guest_DS_limit							0x00004806
#define Guest_FS_limit							0x00004808
#define Guest_GS_limit							0x0000480A
#define Guest_LDTR_limit						0x0000480C
#define Guest_TR_limit							0x0000480E
#define Guest_GDTR_limit						0x00004810
#define Guest_IDTR_limit						0x00004812
#define Guest_ES_access_rights					0x00004814
#define Guest_CS_access_rights					0x00004816
#define Guest_SS_access_rights					0x00004818
#define Guest_DS_access_rights					0x0000481A
#define Guest_FS_access_rights					0x0000481C
#define Guest_GS_access_rights					0x0000481E
#define Guest_LDTR_access_rights				0x00004820
#define Guest_TR_access_rights					0x00004822
#define Guest_interruptibility_state			0x00004824
#define Guest_activity_state					0x00004826
#define Guest_SMBASE							0x00004828
#define Guest_IA32_SYSENTER_CS					0x0000482A

// 32-Bit Host-State Field
#define Host_IA32_SYSENTER_CS					0x00004C00

// Natural-Width Control Fields
#define CR0_guest_host_mask						0x00006000
#define CR4_guest_host_mask						0x00006002
#define CR0_read_shadow							0x00006004
#define CR4_read_shadow							0x00006006
#define CR3_target_value_0						0x00006008
#define CR3_target_value_1						0x0000600A
#define CR3_target_value_2						0x0000600C
#define CR3_target_value_3						0x0000600E

// Natural-Width Read-Only Data Fields
#define Exit_qualification						0x00006400
#define IO_RCX									0x00006402
#define IO_RSI									0x00006404
#define IO_RDI									0x00006406
#define IO_RIP									0x00006408
#define Guest_linear_address					0x0000640A

// Natural-Width Guest-State Fields
#define Guest_CR0								0x00006800
#define Guest_CR3								0x00006802
#define Guest_CR4								0x00006804
#define Guest_ES_base							0x00006806
#define Guest_CS_base							0x00006808
#define Guest_SS_base							0x0000680A
#define Guest_DS_base							0x0000680C
#define Guest_FS_base							0x0000680E
#define Guest_GS_base							0x00006810
#define Guest_LDTR_base							0x00006812
#define Guest_TR_base							0x00006814
#define Guest_GDTR_base							0x00006816
#define Guest_IDTR_base							0x00006818
#define Guest_DR7								0x0000681A
#define Guest_RSP								0x0000681C
#define Guest_RIP								0x0000681E
#define Guest_RFLAGS							0x00006820
#define Guest_pending_debug_exceptions			0x00006822
#define Guest_IA32_SYSENTER_ESP					0x00006824
#define Guest_IA32_SYSENTER_EIP					0x00006826

// Natural-Width Host-State Fields
#define Host_CR0								0x00006C00
#define Host_CR3								0x00006C02
#define Host_CR4								0x00006C04
#define Host_FS_base							0x00006C06
#define Host_GS_base							0x00006C08
#define Host_TR_base							0x00006C0A
#define Host_GDTR_base							0x00006C0C
#define Host_IDTR_base							0x00006C0E
#define Host_IA32_SYSENTER_ESP					0x00006C10
#define Host_IA32_SYSENTER_EIP					0x00006C12
#define Host_RSP								0x00006C14
#define Host_RIP								0x00006C16



/************************
 * STRUCTS
 ************************/

// processor ECX
typedef struct
{
	unsigned int b00_SSE3		:1;
	unsigned int b01_PCLMUL		:1;
	unsigned int b02_DTES64		:1;
	unsigned int b03_MON		:1;
	unsigned int b04_DSCPL		:1;
	unsigned int b05_VMX		:1;
	unsigned int b06_SMX		:1;
	unsigned int b07_EST		:1;
	unsigned int b08_TM2		:1;
	unsigned int b09_SSSE3		:1;
	unsigned int b10_CID		:1;
	unsigned int b11_res		:1;
	unsigned int b12_FMA		:1;
	unsigned int b13_CX16		:1;
	unsigned int b14_ETPRD		:1;
	unsigned int b15_PDCM		:1;
	unsigned int b16_res		:1;
	unsigned int b17_res		:1;
	unsigned int b18_DCA		:1;
	unsigned int b19_SSE4_1		:1;
	unsigned int b20_SSE4_2		:1;
	unsigned int b21_x2APIC		:1;
	unsigned int b22_MOVBE		:1;
	unsigned int b23_POPCNT		:1;
	unsigned int b24_res		:1;
	unsigned int b25_AES		:1;
	unsigned int b26_XSAVE		:1;
	unsigned int b27_OSXSAVE	:1;
	unsigned int b28_AVX		:1;
	unsigned int b29_res		:1;
	unsigned int b30_res		:1;
	unsigned int b31_res		:1;
} CPUID_ECX_t;

// processor EDX
typedef struct
{
	unsigned int b00_FPU		:1;
	unsigned int b01_VME		:1;
	unsigned int b02_DE			:1;
	unsigned int b03_PSE		:1;
	unsigned int b04_TSC		:1;
	unsigned int b05_MSR		:1;
	unsigned int b06_PAE		:1;
	unsigned int b07_MCE		:1;
	unsigned int b08_CX8		:1;
	unsigned int b09_APIC		:1;
	unsigned int b10_res		:1;
	unsigned int b11_SEP		:1;
	unsigned int b12_MTRR		:1;
	unsigned int b13_PGE		:1;
	unsigned int b14_MCA		:1;
	unsigned int b15_CMOV		:1;
	unsigned int b16_PAT		:1;
	unsigned int b17_PSE36		:1;
	unsigned int b18_PSN		:1;
	unsigned int b19_CLFL		:1;
	unsigned int b20_res		:1;
	unsigned int b21_DTES		:1;
	unsigned int b22_ACPI		:1;
	unsigned int b23_MMX		:1;
	unsigned int b24_FXSR		:1;
	unsigned int b25_SSE		:1;
	unsigned int b26_SSE2		:1;
	unsigned int b27_SS			:1;
	unsigned int b28_HTT		:1;
	unsigned int b29_TM1		:1;
	unsigned int b30_IA_64		:1;
	unsigned int b31_PBE		:1;
} CPUID_EDX_t;

// cpuid registers flags
typedef struct
{
	unsigned int eax;	// processor type/family/model/stepping
	unsigned int ebx;	// brand ID is encoded in bits 7...0
	CPUID_ECX_t ecx;	// feature flags
	CPUID_EDX_t edx;	// feature flags
} CPUID_t;

// control register cr0
typedef struct
{
	unsigned int b00_PE			:1;
	unsigned int b01_MP			:1;
	unsigned int b02_EM			:1;
	unsigned int b03_TS			:1;
	unsigned int b04_ET			:1;
	unsigned int b05_NE			:1;
	unsigned int b0615_res		:10;
	unsigned int b16_WP			:1;
	unsigned int b17_res		:1;
	unsigned int b18_AM			:1;
	unsigned int b1928_res		:10;
	unsigned int b29_NW			:1;
	unsigned int b30_CD			:1;
	unsigned int b31_PG			:1;
} CR0_t;

// control register cr3 when CR4.PAE=1 (Physical Address Extension)
typedef struct
{
	unsigned int b0002_res		:3;
	unsigned int b03_PWT		:1;
	unsigned int b04_PCD		:1;
	unsigned int b0531_page_dir	:27;
} CR3_PAE_t;

// control register cr3
typedef struct
{
	unsigned int b0002_res		:3;
	unsigned int b03_PWT		:1;
	unsigned int b04_PCD		:1;
	unsigned int b0511_res		:7;
	unsigned int b1231_page_dir	:10;
} CR3_t;

// control register cr4
typedef struct
{
	unsigned int b00_VME		:1;
	unsigned int b01_PVI		:1;
	unsigned int b02_TSD		:1;
	unsigned int b03_DE			:1;
	unsigned int b04_PSE		:1;
	unsigned int b05_PAE		:1;
	unsigned int b06_MCE		:1;
	unsigned int b07_PGE		:1;
	unsigned int b08_PCE		:1;
	unsigned int b09_OSFXSR		:1;
	unsigned int b10_OSXMMEXCPT	:1;
	unsigned int b11_res		:1;
	unsigned int b12_res		:1;
	unsigned int b13_VMXE		:1;
	unsigned int b14_SMXE		:1;
	unsigned int b1531_res		:17;
} CR4_t;

// processor status flags
typedef struct
{
	unsigned int b00_CF			:1;
	unsigned int b01_one		:1;
	unsigned int b02_PF			:1;
	unsigned int b03_zero		:1;
	unsigned int b04_AF			:1;
	unsigned int b05_zero		:1;
	unsigned int b06_ZF			:1;
	unsigned int b07_SF			:1;
	unsigned int b08_TF			:1;
	unsigned int b09_IF			:1;
	unsigned int b10_DF			:1;
	unsigned int b11_OF			:1;
	unsigned int b12_OPPL		:2;
	unsigned int b14_NT			:1;
	unsigned int b15_zero		:1;
	unsigned int b16_RF			:1;
	unsigned int b17_VM			:1;
	unsigned int b18_AC			:1;
	unsigned int b19_VIF		:1;
	unsigned int b20_VIP		:1;
	unsigned int b21_ID			:1;
	unsigned int b22_zero		:10;
} EFLAGS_t;

/************************
 * GLOBALS VARIABLES
 ************************/
// vmxon data
extern unsigned long long vmxon_ptr;
extern unsigned long long vmcs_ptr;


/************************
 * FUNCTION PROTOTYPES
 ************************/

// get processor feature flags, ecx of cpuid leaf 1
void cpuid(CPUID_t*);

unsigned int vmx_read_cr0();
unsigned int vmx_read_cr2();
unsigned int vmx_read_cr3();
unsigned int vmx_read_cr4();

void vmx_write_cr0(unsigned int);
void vmx_write_cr3(unsigned int);
void vmx_write_cr4(unsigned int);

unsigned int vmx_read_eflags();
void vmx_write_eflags(unsigned int);

void vmx_read_msr(unsigned int msr, unsigned long long* value);
void vmx_write_msr(unsigned int msr, unsigned long long* value);

// enter in vmx hypervisor mode, return true if we are in vmx root mode, otherwise false 
extern unsigned int vmx_vmxon();
extern unsigned int vmx_vmxoff();
extern unsigned int vmx_vmptrld();
extern unsigned int vmx_vmclear();
extern unsigned int vmv_vmread(unsigned int field);
extern void vmv_vmwrite(unsigned int field, unsigned int value);

#endif
