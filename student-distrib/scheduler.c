#include "scheduler.h"
#include "terminal.h"
#include "paging.h"
#include "lib.h"

// void scheduler(void)
// This function implements the scheduler algorithm
// Inputs: none
// Outputs: none
void scheduler(void){
    // need to get current pcb 
    pcb_t *curpcb;
    int inpcb;
    int pidused;
    inpcb = terminal_arr[runTerminal].young_pcb; // process is running
    pidused = terminal_arr[runTerminal].pcb_used[inpcb];
    if(pidused == -1){
        execute((uint8_t *)"shell"); // prepare
        //return;
    }
    *curpcb = get_pcb(pidused);


    if(curpcb != NULL){ // not running ,, go to the next terminal and see what is running ,, get the next pcb
        asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1;"  
        :"=r"(curpcb->ker_ebp), "=r"(curpcb->ker_esp));

    }

    asm volatile(
    "movl %%ebp, %0;"
    "movl %%esp, %1;"  
    :"=r"(curpcb->ker_ebp), "=r"(curpcb->ker_esp)); // this needs to save og process

    // save the esp and ebp 
    // come back restore it
    runTerminal = (runTerminal + 1) % 3; // round robin stuff
    inpcb = terminal_arr[runTerminal].young_pcb; // process is running
    pidused = terminal_arr[runTerminal].pcb_used[inpcb];
    *curpcb = get_pcb(pidused);

    
    // send eoi
    

    // now need to terminal switch and all that jazz
    // do paging // swotch switch
    initialize_program(pidused);
    //tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHTMB - pidused*EIGHTKB - BYTES_4; // b vary!
    scheduleVidMem();

    // need to restore the next process ebp and esp ,, save this process !!
    asm volatile(
    "movl %%ebp, %0;" // flip 0 
    "movl %%esp, %1;"  // flip 1
    :"=r"(curpcb->ker_ebp), "=r"(curpcb->ker_esp));

    return;
}

