// // /* i8259.c - Functions to interact with the 8259 interrupt controller
// //  * vim:ts=4 noexpandtab
// //  */

// // #include "i8259.h"
// // #include "lib.h"

// // /* Interrupt masks to determine which interrupts are enabled and disabled */
// // uint8_t master_mask; /* IRQs 0-7  */
// // uint8_t slave_mask;  /* IRQs 8-15 */

// // /* Initialize the 8259 PIC */
// // void i8259_init(void) {
// // }

// // /* Enable (unmask) the specified IRQ */
// // void enable_irq(uint32_t irq_num) {
// // }

// // /* Disable (mask) the specified IRQ */
// // void disable_irq(uint32_t irq_num) {
// // }

// // /* Send end-of-interrupt signal for the specified IRQ */
// // void send_eoi(uint32_t irq_num) {
// // }

// /* i8259.c - Functions to interact with the 8259 interrupt controller
//  * vim:ts=4 noexpandtab
//  */

// #include "i8259.h"
// #include "lib.h"

// /* Interrupt masks to determine which interrupts are enabled and disabled */
// uint8_t master_mask; /* IRQs 0-7  */
// uint8_t slave_mask;  /* IRQs 8-15 */

// // got most of my information from this website https://wiki.osdev.org/8259_PIC
// /* Initialize the 8259 PIC */
// void i8259_init(void) {
//     master_mask = 0xFF; // 
//     slave_mask = 0xFF; // 

//     // initialize master
//     outb(ICW1, MASTER_8259_PORT);
//     outb(ICW2_MASTER, MASTER_8259_PORT+1);
//     outb(ICW3_MASTER, MASTER_8259_PORT+1);
//     outb(ICW4, MASTER_8259_PORT+1);

//     //initalize slave
//     outb(ICW1, SLAVE_8259_PORT);
//     outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
//     outb(ICW3_MASTER,SLAVE_8259_PORT+1);
//     outb(ICW4, SLAVE_8259_PORT+1);

//     outb(master_mask, MASTER_8259_PORT+1);
//     outb(slave_mask,SLAVE_8259_PORT+1);

//     // need to enable slave interrupts to be processed to masters interrupts
//     enable_irq(ENABLE_SLAVE);
// }

// /* Enable (unmask) the specified IRQ */
// void enable_irq(uint32_t irq_num) {
//     uint16_t correct_port;
//     uint16_t correct_mask;
//     // check the bounderies
//     // for master its 0 to 7 
//     if(irq_num >= 0 && irq_num <= 7){
//         correct_port = (MASTER_8259_PORT)+1;
//         master_mask &= ~(1<<irq_num); // Put 1 on th (irq_num) bit ,, flip mask so all bits but (irq_num) is 1
//         correct_mask = master_mask;
//     }
//     else if(irq_num >7 && irq_num < 16){
//         correct_port = (SLAVE_8259_PORT)+1;
//         slave_mask &= ~(1<<(irq_num-8)); // Put 1 on th (irq_num) bit ,, flip mask so all bits but (irq_num) is 1
//         correct_mask = slave_mask;
//     }
//     outb(correct_mask,correct_port);
// }

// /* Disable (mask) the specified IRQ */
// void disable_irq(uint32_t irq_num) {
//         uint16_t correct_port;
//     uint16_t correct_mask;
//     // check the bounderies
//     // for master its 0 to 7 
//     if(irq_num >= 0 || irq_num <= 7){
//         correct_port = (MASTER_8259_PORT)+1;
//         master_mask = inb(correct_port)|(1<<irq_num); // Put 1 on th (irq_num)th bit,, set the bit mask
//         correct_mask = master_mask;
//     }
//     else if(irq_num >7 || irq_num < 16){
//         correct_port = (SLAVE_8259_PORT)+1;
//         slave_mask = slave_mask | (1<<(irq_num-8)); //Put 1 on th (irq_num)th bit,, set the bit mask
//         correct_mask = slave_mask;
//     }
//     outb(correct_mask,correct_port);
// }

// /* Send end-of-interrupt signal for the specified IRQ */
// void send_eoi(uint32_t irq_num) {
//     uint16_t correct_port;
//     if(irq_num >= 0 && irq_num <= 7){
//         correct_port = MASTER_8259_PORT;
//         outb(EOI | irq_num, correct_port );
//     }
//     else if(irq_num > 7 && irq_num < 16){
//         correct_port = SLAVE_8259_PORT;
//         outb( EOI | (irq_num - 8), correct_port); 
//  		outb( EOI + 2, correct_port); 

//     }
// }


// --
/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

// got most of my information from this website https://wiki.osdev.org/8259_PIC
/* Initialize the 8259 PIC */
// void i8259_init(void)
// Initializes the Master and Slave PIC
// Inputs: None
// Outputs: None
void i8259_init(void) {
    master_mask = MASK; // Master PIC Mask
    slave_mask = MASK; // Slave PIC Mask

    // initialize master
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_PORT+1);
    outb(ICW3_MASTER, MASTER_8259_PORT+1);
    outb(ICW4, MASTER_8259_PORT+1);

    //initalize slave
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
    outb(ICW3_SLAVE,SLAVE_8259_PORT+1);
    outb(ICW4, SLAVE_8259_PORT+1);

    outb(master_mask, MASTER_8259_PORT+1);
    outb(slave_mask,SLAVE_8259_PORT+1);

    // need to enable slave interrupts to be processed to masters interrupts
    enable_irq(ENABLE_SLAVE);
}

/* Enable (unmask) the specified IRQ */
// void enable_irq(uint32_t irq_num)
// Enables the interrupts
// Inputs: irq_num: interrupt number
// Outputs: None
void enable_irq(uint32_t irq_num) {
    uint16_t correct_port;
    uint16_t correct_mask;
    // check the bounderies
    // for master its 0 to 7 
    if(irq_num >= IRQ_START && irq_num <= IRQ_MID){ // checking the bounds
        correct_port = (MASTER_8259_PORT)+1;
        master_mask &= ~(1<<irq_num); // Put 1 on th (irq_num) bit ,, flip mask so all bits but (irq_num) is 1
        correct_mask = master_mask;
    }
    else if(irq_num >IRQ_MID && irq_num < IRQ_END){ // checking the bounds
        correct_port = (SLAVE_8259_PORT)+1;
        slave_mask &= ~(1<<(irq_num-8)); // Put 1 on th (irq_num) bit ,, flip mask so all bits but (irq_num) is 1
        correct_mask = slave_mask;
    }
    outb(correct_mask,correct_port);
}

/* Disable (mask) the specified IRQ */
// void disable_irq(uint32_t irq_num) 
// Disables the interrupts
// Inputs: irq_num: interrupt number
// Outputs: None
void disable_irq(uint32_t irq_num) {
        uint16_t correct_port;
    uint16_t correct_mask;
    // check the bounderies
    // for master its 0 to 7 
    if(irq_num >= IRQ_START || irq_num <= IRQ_MID){ // checking the bounds 
        correct_port = (MASTER_8259_PORT)+1;
        master_mask = inb(correct_port)|(1<<irq_num); // Put 1 on th (irq_num)th bit,, set the bit mask
        correct_mask = master_mask;
    }
    else if(irq_num > IRQ_MID || irq_num < IRQ_END){ // checking the bounds
        correct_port = (SLAVE_8259_PORT)+1;
        slave_mask = slave_mask | (1<<(irq_num-8)); //Put 1 on th (irq_num)th bit,, set the bit mask
        correct_mask = slave_mask;
    }
    outb(correct_mask,correct_port);
}

/* Send end-of-interrupt signal for the specified IRQ */
// void send_eoi(uint32_t irq_num) 
// Sends an end of interrupt signal to the PIC
// Inputs: irq_num: interrupt number
// Outputs: None
void send_eoi(uint32_t irq_num) {
    uint16_t correct_port;
    if(irq_num >= IRQ_START && irq_num <= IRQ_MID){ // checking the bounds
        correct_port = MASTER_8259_PORT;
        outb(EOI | irq_num, correct_port );
    }
    else if(irq_num > IRQ_MID && irq_num < IRQ_END){ // checking the bounds 
        correct_port = SLAVE_8259_PORT;
        outb( EOI | (irq_num - 8), correct_port); 
 		outb( EOI + 2, MASTER_8259_PORT); // we need one for the master port,, and we need one for the slave port

    }
}
