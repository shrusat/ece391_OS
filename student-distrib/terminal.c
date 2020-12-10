/*
Terminal.c
*/
#include "terminal.h"

// int terminal_open(int32_t *fd)
// This function initializes the terminal and keyboard
// Inputs: fd = file name
// Outputs: 0 if success and -1 if failed
int32_t terminal_open(const uint8_t *fd){
    clear();
    return 0;
}

// int terminal_close(int32_t *fd)
// This function exits the terminal 
// Inputs: fd = file name
// Outputs: 0 if success and -1 if failed
int32_t terminal_close(int32_t fd){
    return 0;
}

// int terminal_read(int32_t *fd, char buf[], int nbytes)
// This function reads keyboard buffer
// Inputs: fd = file name, buf = buffer, nbytes = number of bytes to read
// Outputs: Returns number of bytes read
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    //Check nbytes
    if(nbytes < 0){
        return -1;
    }
    //Check when buf is not null (use strlen)
    while(1){
        if(keyBuffer[strlen(keyBuffer)-1] == '\n'){
            break;
        }
    }

    int i;
    int num = 0;
    //Read terminal buffer
    for(i = 0; i < nbytes; i++){
        ((int8_t *)buf)[i] = keyBuffer[i];
        num++;
        if(keyBuffer[i] == '\n'){
            break;
        }
    }
    ((int8_t *)buf)[num] = '\0';
    clearBuffer();
    return num;
}

// int terminal_write(int32_t *fd, char buf[], int nbytes)
// This function writes keyboard buffer to screen
// Inputs: fd = file name, buf = buffer, nbytes = number of bytes to read
// Outputs: Returns number of bytes written, -1 if not correct buffer
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    //Check arguments
    // if(strlen((int8_t *)buf)== 0){
    //     return -1;
    // }
    if(nbytes < 0){
        return -1;
    }
    //Print buffer
    int i;
    for(i = 0; i < nbytes; i++){
        putc(((int8_t *)buf)[i]);
    }
    return nbytes; 
}

// void terminal_init()
// This function initialize the 3 terminals
// Inputs: none
// Outputs: none
void terminal_init(){
    currTerminal = 0; //First terminal
    
    //Initialize all terminal struct variables
    int j;
    for(j = 0; j < MAX_TERM; j++){
        int i;
        for(i = 0; i < MAX_PCBS; i++){ 
            terminal_arr[j].pcb_used[i] = -1;
        }
    
        terminal_arr[j].terminal_number = j;
        terminal_arr[j].curr_buff = 0;

        terminal_arr[j].cursor_x = 0;
        terminal_arr[j].cursor_y = 0;

        terminal_arr[j].young_pcb = 0;
    }
}

// void switch_terminal(int new_terminal)
// This function switches the screen to the new terminal (called after alt + f#)
// Inputs: new_terminal = terminal index to switch to
// Outputs: none
void switch_terminal(int new_terminal){
    runTerminal = currTerminal; //Not for scheduling

    //Change screen
    switchVidMem(currTerminal, new_terminal);

    currTerminal = new_terminal;
    runTerminal = currTerminal;

    //Not for scheduling
    pcb_t pcb;
    pcb = get_pcb(new_terminal); // terminal 0 is for pcb 0, terminal 1 is for pcb 1, terminal 2 is for pcb 2
    if(pcb.file_array[0].file_op_table_ptr == NULL){
        execute((uint8_t*)("shell"));
    }

    //call context switch to current terminal
    //scheduler();
}

// terminal_t get_current_terminal()
// This function gets the terminal struct from the terminal array
// Inputs: none
// Outputs: none
terminal_t get_current_terminal(){
    return terminal_arr[currTerminal];
}

// update_initial_pcb(uint32_t pid)
// This function updates the pcb in the terminal struct
// Inputs: pid = id of the pcb
// Outputs: none

// used to launch initial terminals 0, 1, and 2
// terminal 0 = pid 0
// terminal 1 = pid 1
// terminal 2 = pid 2
void update_initial_pcb(uint32_t pid){
    terminal_arr[pid].pcb_used[0] = pid;
}

// void update_pcb_used(int index, uint32_t pid)
// This function updates the pcb in the terminal struct with the index
// Inputs: index = the index in the pcb_used array, pid = id of the pcb
// Outputs: none
void update_pcb_used(int index, uint32_t pid){
    terminal_arr[currTerminal].pcb_used[index] = pid;
    terminal_arr[currTerminal].young_pcb = index;
}



