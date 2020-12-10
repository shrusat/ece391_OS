#include "types.h"
#include "x86_desc.h"
#include "lib.h"
// this is where we will declare our functions for paging

#define EIGHT_MB   0x800000
#define FOUR_MB    0x400000
#define ONETWOEIGHT_MB 0x8000000
#define ONETHREETWO_MB 0X8400000

//Magic Numbers
#define PAGES 1024
#define READ_WRITE 0x0002
#define PRESENT 0x001
#define TABLE_NUM 4096
#define NULL_NUM 0x0
#define NUM_4MB 0x00400000
#define DIR_NUM 0x83
#define VM_NUM 0xB8
#define VB1_NUM 0xB9
#define VB2_NUM 0xBA
#define VB3_NUM 0xBB
#define USER 0x004
#define SIZE 0x0080


// initialize the arrays
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
uint32_t pageTable[1024] __attribute__((aligned (4096)));
uint32_t userPageTable[1024] __attribute__((aligned (4096)));

//initialize functions
extern void initialize_paging(void);
void load_directory(unsigned int*);
void page_enabler();

void initialize_program(uint32_t pid);

void remap_page(uint32_t virtual_address, uint32_t physical_addrress);

void flush_tlb(void);


