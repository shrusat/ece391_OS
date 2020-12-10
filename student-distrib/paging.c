#include "paging.h" 

// this is where we need to write code for paging
// we need to create the page directory and page table

//Code Here


//This function initializes paging
//What it does it takes the page directory and page table, and maps the correct values towards it
//It will initially set everything to read write
// But then accordinly it will set things to present or turn on the size bit
void initialize_paging(void){
    int i; // this will traverse thru everything

    for(i=0 ; i < PAGES; i++){ // 1024 is from 2^10
        pageDirectory[i] = READ_WRITE; // set it to read and write ,, not present 

        pageTable[i] = (i*TABLE_NUM)|READ_WRITE; // read and write // 0-4mb // 4 bytes -- 1kb pages 
        userPageTable[i] = READ_WRITE;
    }

    pageTable[0] = NULL_NUM; // set to NULL

    // first entry is for the page table
    pageDirectory[0] = ((uint32_t)pageTable) | (READ_WRITE) | (PRESENT); // set to read write and present
    // need to figure out if its present / read/write
    //pageDirectory[0] |= (0x00000003); // 1 bit is on for present, 1 bit is on for read/write 

    // second entry is for the kernel stuff
    // top 10 bits need to map to 4MB
    // user should not have access
    // x00400000 came from discussion,, 4MB = 1x2^22
    // we need to have the read/write bit on, enable bit on, supervisor bit off, and size bit on
    pageDirectory[1] = (NUM_4MB) | (DIR_NUM); // 4MB ,, set r/w, present, size on

    // we then need to allign the page table w the correct video memory address 
    // top 20 bits need to map to 4KB
    
   
    pageTable[VM_NUM] |= 3; //0xB8 came from discussion,, points to the video memory // or it by 3 bc of read write and present bit
    pageTable[VB1_NUM] |= 3; //For video buffer 1
    pageTable[VB2_NUM] |= 3; //For video buffer 2
    pageTable[VB3_NUM] |= 3; //For video buffer 3

    //clear();
    // call assembly functions
    load_directory(pageDirectory);
    //page_enabler();

    

        
}

//alocate page for the user program 
// pid is the number of the process ,, process id basically
void initialize_program(uint32_t pid){
    // 8mb for the first program, +4 for the next and etc 

    uint32_t address = EIGHT_MB + pid*FOUR_MB;

    // 128/4 = 32 
    pageDirectory[32] = address | SIZE | USER | READ_WRITE| PRESENT; // size, user, read/write, present 
    flush_tlb();
}
/*
 *   Function: remap_page
 *   description: This function takes virtutal and physical addresses and maps the 4MB chunk of memory (a single PDE)
 *                begining at the given virtual address to the first page in the newly created userPageTab;e
 *   inputs: virtual_address - multiple of 4MB virtual address to be mapped
 *           physical_addrress - physical address to be mapped
 *   outputs: none
 *
 */
void remap_page(uint32_t virtual_address, uint32_t physical_addrress){
    uint32_t entry = virtual_address / NUM_4MB; // divide by 4mb page 
   pageDirectory[entry] = ((uint32_t)userPageTable) | USER| READ_WRITE| PRESENT ; // user, read/write, present 
    userPageTable[0] = (physical_addrress)|USER| READ_WRITE| PRESENT; // user, read/write, present // fill first entry
    
    flush_tlb(); 
}

void flush_tlb(void){
    asm volatile(
                 "mov %%cr3, %%eax;"
                 "mov %%eax, %%cr3;"
                 :                      /* no outputs */
                 :                      /* no inputs */
                 :"%eax"                /* clobbered register */
                 );
}