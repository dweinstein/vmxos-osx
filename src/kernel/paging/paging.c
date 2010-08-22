// from http://www.osdever.net/tutorials/paging.php?the_id=43

#include "paging.h"

// setting up the Page Directory
unsigned long *page_directory = (unsigned long *) 0x9C000;

//Setting up the Page Table
unsigned long *page_table = (unsigned long *) 0x9D000; // the page table comes right after the page directory

void enable_paging()
{
	// filling in the page table
	unsigned long address=0; // holds the physical address of where a page is
	unsigned int i;
	// map the first 4MB of memory
	for(i=0; i<1024; i++)
	{
		page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
		address = address + 4096; // 4096 = 4kb
	}

	// filling in the Page Directory Entries
	// the first entry of the page directory
	page_directory[0] = UINT(page_table); // attribute set to: supervisor level, read/write, present(011 in binary)
	page_directory[0] = page_directory[0] | 3;
	// the other 1023
	for(i=1; i<1024; i++)
	{
		page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
	}

	// write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions
	vmx_write_cr3(UINT(page_directory)); // put that page directory address into CR3
	vmx_write_cr0(vmx_read_cr0() | 0x80000000); // set the paging bit in CR0 to 1
}
