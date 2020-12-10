#include "rtc.h"


int rtc_interrupt_occured;
//static int start_y = 1;

// void initialize_rtc(void)
// This function initializes the rtc and enables the interrupt.
// Inputs: None
// Outputs: None
void initialize_rtc(void){

    char prev_reg;
    unsigned char rate = 0x0F; // must be above 2 and not over 15 
    unsigned char mask = 0xF0; // mask value for register A
    //disable_ints();			// disable interrupts
    
    cli();
    outb(DISABLE_B, RTC_INDEX_1);		// select register B, and disable NMI
    prev_reg= inb(RTC_RW );	// read curr value of register B
    outb(DISABLE_B, RTC_INDEX_1);		// set the index again (a read will reset the index to register D)
    outb( prev_reg | 0x40, RTC_RW );	// write the previous value ORed with 0x40. This turns on bit 6 of register B
    //enable_ints();
    
    sti();
    enable_irq(RTC_IRQ);
    cli();
    
    outb(DISABLE_A, RTC_INDEX_1);
    prev_reg = inb(RTC_RW);
    outb(DISABLE_A, RTC_INDEX_1);
    outb((prev_reg & (mask))| rate, RTC_RW);

    rtc_interrupt_occured = 0;
    change_frequency(DEFAULT_FREQ); //default rtc frequency = 2 Hz
}

// void rtc_handler(void)
// This function handles the rtc interrupts and calls 
// test_interrupts to show that RTC receives interrupts
// Checkpoint 1: full interrupt handler not written yet 
// Inputs: None
// Outputs: None
void rtc_handler(void){
    cli();
    send_eoi(RTC_IRQ);
    // test_interrupts();
    // if(start_y == 1){ // check where the character needs to be printed
    //     putc('1'); // print at index (0,0)
    //     start_y = 0;
    // } else {
    //     putc('1'); // print at current location on screen
    // }
    
    rtc_interrupt_occured = 0;

    outb(RC, RTC_INDEX_1); //register C
    inb(RTC_RW); 	

    sti();   
}

// void change_frequency(uint32_t frequency)
// This function changes the frequency of RTC  
// Inputs: frequency = frequency that needs to be changed
// Outputs: None
void change_frequency(uint32_t frequency){
    //max rate = 1024 Hz = (32768 >>(6 - 1))
    uint32_t rate;
    if((frequency > 0) && ((frequency & (frequency - 1)) == 0)){ //check if greater than 0 and power of 2
        // uint32_t rate = log(32768.0/frequency) + 1; 
        // 32768/512 = 64
        // log(64) = 6
        // 6+1
        
        switch(frequency){
            case 2: rate = RATE_2; break;
            case 4: rate = RATE_4; break;
            case 8: rate = RATE_8; break;
            case 16: rate = RATE_16; break;
            case 32: rate = RATE_32; break;
            case 64: rate = RATE_64; break;
            case 128: rate = RATE_128; break;
            case 256: rate = RATE_256; break;
            case 512: rate = RATE_512; break; // 7
            case 1024: rate = RATE_1024; break; // max rate = 1024 Hz = (32768 >>(6 - 1))
            default: return; // return -1 if frequency is below 2 Hz or above 1024 Hz
        }
    }else{
        return;
    }

    cli();
    // change frequency (32768 >> (rate-1)) = frequency
    outb(DISABLE_A, RTC_INDEX_1);
    char reg_a = inb(RTC_RW);
    outb(DISABLE_A, RTC_INDEX_1);
    outb((reg_a & (0xF0))| rate, RTC_RW);
    // outb((reg_a & 0xF0) | frequency, 0x71);
    sti();
    return;
}
// rtc_open: provides access to the file system.
// 1) find dir entry corresponding to the named file
// 2) allocate an unused file descriptor
// 3) set up any data necessary to handle given type of file
// return -1 if named file DNE or no descriptors are free
int32_t rtc_open(const uint8_t * filename){
    change_frequency(2);
    return 0;
}
// rtc_close: closes specified file descriptor, makes 
// it available for return from later calls to open
// do NOT allow user to close default descriptors (0=I, 1=O)
// return -1 if user tries to close invalid descriptor. 
// return 0 for success
int32_t rtc_close(int32_t fd){
    return 0;
}
// rtc_read: need to synchronize with interrupt handlers
// should wait until next RTC interrupt has occured before it returns.
// use simple volatile flag variables to do this synchronization (e.g.
// int rtc_interrupt_occured;) whenever possible and small critical 
// sections with cli/sti.
int32_t rtc_read(int32_t fd, void * buf, int32_t n_bytes){
    cli();
    rtc_interrupt_occured = 1;
    sti();
    while(rtc_interrupt_occured){
        continue;
    }
    return 0;
}
// rtc_write: writing to RTC should block 
// interrupts to interact with the device
int32_t rtc_write(int32_t fd, const void * buf, int32_t n_bytes){
    uint32_t freq = *(uint8_t*)buf; //cast and dereference to find frequency value
    change_frequency(freq); //boundary checks performed in check_frequency
    return 0;
}



// i got all this info from https://wiki.osdev.org/RTC 
