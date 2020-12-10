/*
Terminal.h
*/

#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "sys_call.h"
#include "scheduler.h"

#define MAX_TERM 3

int currTerminal;
int runTerminal;
terminal_t terminal_arr[MAX_TERM]; 

terminal_t get_current_terminal();
void update_initial_pcb(uint32_t pid);
void update_pcb_used(int index, uint32_t pid);

//Initializes terminal
int32_t terminal_open(const uint8_t *fd);
//Exits terminal
int32_t terminal_close(int32_t fd);
//Reads buffer
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
//Writes to terminal
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
//Initialize terminal
void terminal_init();
//Helper function to switch terminals
void switch_terminal(int new_terminal);

