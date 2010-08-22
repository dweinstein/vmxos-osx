#ifndef _VMX_H_
#define _VMX_H_

// processor feature flags
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

// get processor feature flags, ecx of cpuid leaf 1
unsigned int cpuid();

unsigned int vmx_read_cr0();
unsigned int vmx_read_cr3();
unsigned int vmx_read_cr4();

void vmx_write_cr0(unsigned int);
void vmx_write_cr3(unsigned int);
void vmx_write_cr4(unsigned int);

unsigned int vmx_read_eflags();
void vmx_write_eflags(unsigned int);

#endif
