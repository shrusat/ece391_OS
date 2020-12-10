/*
Keyboard.c
*/
#include "keyboard.h"

//Global Variable
static int shiftButton = UNPRESSED;
static int capslockButton = UNPRESSED;
static int ctrlButton = UNPRESSED;
static int altButton = UNPRESSED;

//No shift or caps
static char mapNormal[KEY_SIZE] = {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
	                    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
	                    'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 
	                    'b', 'n', 'm',',', '.', '/', '\0', '\0', '\0', ' '};

//Shift map
static char mapShift[KEY_SIZE] = {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
	                    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\0', '\0', 'A', 'S',
	                    'D', 'F', 'G', 'H', 'J', 'K', 'L' , ':', '\"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 
	                    'B', 'N', 'M','<', '>', '?', '\0', '\0', '\0', ' '};

//Caps Lock map
static char mapCapsLock[KEY_SIZE] = {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
	                    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\0', '\0', 'A', 'S',
	                    'D', 'F', 'G', 'H', 'J', 'K', 'L' , ';', '\'', '`', '\0', '\\', 'Z', 'X', 'C', 'V', 
	                    'B', 'N', 'M',',', '.', '/', '\0', '\0', '\0', ' '};

//Shift and Caps Lock
static char mapShiftCapsLock[KEY_SIZE] = {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
	                    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\0', '\0', 'a', 's',
	                    'd', 'f', 'g', 'h', 'j', 'k', 'l' , ':', '\"', '~', '\0', '|', 'z', 'x', 'c', 'v', 
	                    'b', 'n', 'm','<', '>', '?', '\0', '\0', '\0', ' '};

// void initialize_keyboard(void)
// This function initializes the keyboard and enables the interrupt.
// Inputs: None
// Outputs: None
void initialize_keyboard(void){
    enable_irq(ENABLE_KB_IRQ); //Enables the keyboard interrupt
    keyIndex = 0;
    int i;
    for(i = 0; i < BUFF_SIZE; i++){
        keyBuffer[i] = NULL_CODE;
    }
}

// void handle_keyboard(void)
// This function handles the keys on the keyboard and prints the key that was typed
// Checkpoint 1: Only works for letters and numbers 
// Checkpoint 2: Works for CTRL, ALT, TAB, ENTER, BACKSPACE, SHIFT, CAPSLOCK
// Inputs: None
// Outputs: None
void handle_keyboard(void){
    send_eoi(ENABLE_KB_IRQ);
    cli();
    int key = 0;

    while(1){
        key = inb(KB_DATA_PORT); //Takes the scancode from the keyboard
        if(key != 0){
            break;
        }
    };

    int i;
    switch (key){
    //Check if the shift button is pressed
    case RSHIFT_PRESSED:
        shiftButton = PRESSED;
        break;
    case LSHIFT_PRESSED:
        shiftButton = PRESSED;
        break;
    case RSHIFT_UNPRESSED:
        shiftButton = UNPRESSED;
        break;
    case LSHIFT_UNPRESSED:
        shiftButton = UNPRESSED;
        break;

    //Check if caps lock button is pressed
    case CAPSLOCK:
        if(capslockButton == UNPRESSED){
            capslockButton = PRESSED;
        }
        else{
            capslockButton = UNPRESSED;
        }
        break;

    //Backspace
    case BACKSPACE:
        if(keyIndex > 0){
            keyBuffer[keyIndex-1] = NULL_CODE;
            backspace();
            keyIndex--;
        }
        break;

    //Tab
    case TAB:
        for (i = 0; i < 3; i++){
            if(keyIndex > (BUFF_SIZE-2)){
                break;
            }
            keyBuffer[keyIndex] = ' '; //Space character
            putc(keyBuffer[keyIndex]);
            keyIndex++;
        }
        break;

    //Enter
    case ENTER:
        if(keyIndex < BUFF_SIZE){
            keyBuffer[keyIndex] = '\n'; //Enter character
            keyIndex++;
            putc(keyBuffer[keyIndex-1]);
        }
        keyIndex = 0;
        break;

    //Ctrl
    case CTRL_PRESSED:
        ctrlButton = PRESSED;
        break;
    case CTRL_UNPRESSED:
        ctrlButton = UNPRESSED;
        break;

    //Alt
    case ALT_PRESSED:
        altButton = PRESSED;
        break;
    case ALT_UNPRESSED:
        altButton = UNPRESSED;
        break;
    
    //F#
    case F1:
        if(altButton){
            switch_terminal(0); //Terminal 1 
        }
        break;
    case F2:
        if(altButton){
            switch_terminal(1); //Terminal 2 
        }
        break;
    case F3:
        if(altButton){
            switch_terminal(2); //Terminal 3ir
        }
        break;

    default:
        //Key presses 
        if(key < KEY_SIZE){ //If it is not a letter or number it will return
            if(keyIndex < (BUFF_SIZE-2)){
                keyPresses(key);
                //Print to terminal
                putc(keyBuffer[keyIndex-1]);
            } 
        }
        break;
    }

    sti();
}

// void keyPresses(int key)
// This function handles the keys on the keyboard and if it is shifted or cap locked
// Inputs: key = scancode for the key pressed
// Outputs: None
void keyPresses(int key){
    //Finds key from the array
    char keycode = NULL_CODE; 
    if(capslockButton == PRESSED && shiftButton == PRESSED){
        keycode = mapShiftCapsLock[key];
    }
    else if(shiftButton == PRESSED){
        keycode = mapShift[key];
    }
    else if(capslockButton == PRESSED){
        keycode = mapCapsLock[key];
    }
    else{
        keycode = mapNormal[key];   
    }
    
    if(keycode == NULL_CODE){
        return;
    }

    if(ctrlButton == PRESSED && (keycode == L_BUTTON || keycode == CAPL_BUTTON)){
        clear();
        clearBuffer();
        return;
    }

    keyBuffer[keyIndex] = keycode;
    keyIndex++;
}

// void clearBuffer()
// This function clears the buffer 
// Inputs: none
// Outputs: None
void clearBuffer(){
    int i;
    for(i = 0; i < BUFF_SIZE; i++){
        keyBuffer[i] = NULL_CODE;
    }
    keyIndex = 0;
}


