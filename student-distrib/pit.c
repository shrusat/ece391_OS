#include "pit.h"

/*
* initialize_pit
* This function initializes the pit and enables interrupts to the kernel
* Inputs: none
* Outputs: none
*/
void initialize_pit(void){
    uint8_t low_byte;
    uint8_t high_byte;

    outb(PIT_MODE, PIT_COMMAND); // load PIT mode into Mode/Command register at address 0x43

    // load low 8 bits and high 8 bits sequentially into Channel 0 (this is becuase PIT_MODE uses lobyte/hibyte mode)
    low_byte = PIT_DIVISOR & LOW_MASK; // only low 8 bits are in low_byte
    high_byte = (PIT_DIVISOR & HIGH_MASK) >> 8; // shift right by 8 so high 8 bits are moved to low 8 bits
    outb(low_byte, PIT_CHANNEL_0);
    outb(high_byte, PIT_CHANNEL_0);

    enable_irq(PIT_IRQ); // enable pit interrupts at IRQ0
}

/*
* initialize_pit
* This function handles pit interrupts which is used for multiple terminals
* Inputs: none
* Outputs: none
*/
void pit_handler(void){
    cli();
    send_eoi(PIT_IRQ);
    // Scheduling algorithm goes here
    //scheduler();
    //printf("|");
    sti();
}
