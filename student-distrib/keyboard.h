/*
Keyboard.h - Defines functions to handle the keyboard
*/
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "i8259.h"
#include "lib.h"
#include "terminal.h"

//Magic Numbers
#define KEY_SIZE 59 

#define RSHIFT_PRESSED 54
#define LSHIFT_PRESSED 42
#define RSHIFT_UNPRESSED 182
#define LSHIFT_UNPRESSED 170

#define CTRL_PRESSED 29
#define CTRL_UNPRESSED 157

#define ALT_PRESSED 56
#define ALT_UNPRESSED 184

#define F1 0x3B
#define F2 0x3C
#define F3 0x3D

#define CAPSLOCK 58
#define BUFF_SIZE 128
#define BACKSPACE 14
#define TAB 15 
#define ENTER 28

#define UNPRESSED 0
#define PRESSED 1

#define NULL_CODE '\0'
#define L_BUTTON 'l'
#define CAPL_BUTTON 'L'
#define KB_DATA_PORT 0x60 //Read/Write Data Port
#define ENABLE_KB_IRQ 1 //Enables interrupt
#define TERM_SIZE 3

//keyboard buffer
char keyBuffer[BUFF_SIZE];
int keyIndex;

//Functions
//Initializes keyboard
extern void initialize_keyboard(void);
//Handles button presses
extern void handle_keyboard(void);

void keyPresses(int key);
void clearBuffer();

#endif