// This function initializes the IDT table according to Intel's manual
# include "idt.h" 
# include "x86_desc.h"
# include "handler.h"
#include "system_call_linkage.h"

// void init_idt(void)
// This function initializes the IDT table according to the Intel manual's standards and loads the IDT
// Indices 0 - 31 are reserved for Intel exceptions (only 0 - 19 are defined, excluding 15)
// Indices 32 - 255 are user defined interrupts
// Index 0x80 is reserved for system calls
// Inputs: None
// Outputs: None
void init_idt(void){
    int i; // iteration variable

    // load idt using x86_desc.h macro
    lidt(idt_desc_ptr);

    // initialize all IDT entries with the struct values
    for(i = 0; i < NUM_VEC; i++){

        if((i < 32) && (i >= 0)){ // For Intel exceptions, 0 is the first index, 
                                    // 32 is the index where user defined interrupts start

            // Trap Gate is used for exceptions
            // Bits [12:7] are 0 D 1 1 1 0 according to Intel gate descriptors, this corresponds to
            // reserved0 = 0, reserved1 = 1, reserved2 = 1, reserved3 = 1, and reserved4 = 0

            idt[i].seg_selector = KERNEL_CS;
            idt[i].reserved4 = 0; // see lines 23-25 for why this bit is set
            idt[i].reserved3 = 1; // see lines 23-25 for why this bit is set
            idt[i].reserved2 = 1; // see lines 23-25 for why this bit is set
            idt[i].reserved1 = 1; // see lines 23-25 for why this bit is set
            idt[i].size      = 1; // size of gate: 1 for 32 bits, 0 for 16 bits -> 8 byte gates for IA-32
            idt[i].reserved0 = 0;   // see lines 23-25 for why this bit is set
            idt[i].dpl       = 0; // exception handlers set DPL bit set to 0
            idt[i].present   = 1; // 1 since KERNEL_CS is present
        }
        else if(i == SYSTEM_CALL_INDEX){ // 0x80 is reserved for system calls 

            // Trap Gate is used for 0x80. int $0x80 instruction functions similar to an exception (TRAP Gate)
            // Bits [12:7] are 0 D 1 1 1 0 according to Intel gate descriptors, this corresponds to
            // reserved0 = 0, reserved1 = 1, reserved2 = 1, reserved3 = 1, and reserved4 = 0

            idt[i].seg_selector = KERNEL_CS;
            idt[i].reserved4 = 0; // see lines 39-41 for why this bit is set
            idt[i].reserved3 = 1; // see lines 39-41 for why this bit is set
            idt[i].reserved2 = 1; // see lines 39-41 for why this bit is set
            idt[i].reserved1 = 1; // see lines 39-41 for why this bit is set
            idt[i].size      = 1; // size of gate: 1 for 32 bits, 0 for 16 bits -> 8 byte gates for IA-32
            idt[i].reserved0 = 0; // see lines 39-41 for why this bit is set
            idt[i].dpl       = SYSTEM_CALL_PRIVILEGE; // privilege level is set to 3
            idt[i].present   = 1; // 1 since KERNEL_CS is present
        }
        else { // fill in the rest of the user-defined interrupts (index 32 through 255, excluding x80)

            // Interrupt Gate used for external (hardware generated) interrupts 
            // Table 5-1 defines indexes 32-255 as external interrupts
            // Bits [12:7] are 0 D 1 1 0 0 according to Intel gate descriptors, this corresponds to
            // reserved0 = 0, reserved1 = 1, reserved2 = 1, reserved3 = 0, and reserved4 = 0
              
            //offset_15_00 = 0;
            idt[i].seg_selector = KERNEL_CS;
            idt[i].reserved4 = 0; // see lines 55-58 for why this bit is set
            idt[i].reserved3 = 0; // see lines 55-58 for why this bit is set
            idt[i].reserved2 = 1; // see lines 55-58 for why this bit is set
            idt[i].reserved1 = 1; // see lines 55-58 for why this bit is set
            idt[i].size      = 1; // size of gate: 1 for 32 bits, 0 for 16 bits -> 8 byte gates for IA-32
            idt[i].reserved0 = 0; // see lines 55-58 for why this bit is set
            idt[i].dpl       = 0; // privilege level is set to 0 for hardware interrupt handlers
            idt[i].present   = 1; // 1 since KERNEL_CS is present 
        }
    }
        // set idt entries based on Intel specs in 5.14
        // handlers ending in 'exc' are named according to the abbreviations 
        // given by the Intel documentation
        SET_IDT_ENTRY(idt[0], de_exc); // 0 corresponds to index of divide error exception for Intel
        SET_IDT_ENTRY(idt[1], db_exc); // 1 corresponds to index of debug exception for Intel
        SET_IDT_ENTRY(idt[2], nmi_int); // ... and so on for each of these indices
        SET_IDT_ENTRY(idt[3], bp_exc);
        SET_IDT_ENTRY(idt[4], of_exc);
        SET_IDT_ENTRY(idt[5], br_exc);
        SET_IDT_ENTRY(idt[6], ud_exc);
        SET_IDT_ENTRY(idt[7], nm_exc);
        SET_IDT_ENTRY(idt[8], df_exc);
        SET_IDT_ENTRY(idt[9], cs_overrun);
        SET_IDT_ENTRY(idt[10], ts_exc);
        SET_IDT_ENTRY(idt[11], np_exc);
        SET_IDT_ENTRY(idt[12], ss_exc);
        SET_IDT_ENTRY(idt[13], gp_exc);
        SET_IDT_ENTRY(idt[14], pf_exc);
        // no interrupt 15 in documentation
        SET_IDT_ENTRY(idt[16], mf_exc);
        SET_IDT_ENTRY(idt[17], ac_exc);
        SET_IDT_ENTRY(idt[18], mc_exc);
        SET_IDT_ENTRY(idt[19], xf_exc);

        // Set system call interrupt
        SET_IDT_ENTRY(idt[SYSTEM_CALL_INDEX], system_call_asm); // x80 is reserved for system calls

        // Set Keyboard, RTC, and PIC interrupts
        SET_IDT_ENTRY(idt[KEYBOARD_INDEX], kb_interrupt);
        SET_IDT_ENTRY(idt[RTC_INDEX], rtc_interrupt);
        SET_IDT_ENTRY(idt[PIT_INDEX], pit_interrupt);
}
