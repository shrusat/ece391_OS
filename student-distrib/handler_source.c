// Is called by handler.S to deal with exceptions/interrupts. This prints the correct
// error to the screen and freezes the program with an infinite loop.

#include "handler_source.h"
#include "lib.h"
#include "keyboard.h"
#include "rtc.h"

#include "sys_call.h"

// void divide_error_exception(void)
// This function calls the divide error exception
// Inputs: None
// Outputs: None
void divide_error_exception(){ 
    //clear();
    char* msg = "Error Message: Divide Error Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void debug_exception(void)
// This function calls the debug_exception
// Inputs: None
// Outputs: None
void debug_exception(){ 
    //clear();
    char* msg = "Error Message: Debug Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void nmi_interrupt(void)
// This function calls the nmi interrupt
// Inputs: None
// Outputs: None
void nmi_interrupt(){
    //clear();
    char* msg = "Error Message: NMI Interrupt\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void breakpoint_exception(void)
// This function calls the breakpoint exception
// Inputs: None
// Outputs: None
void breakpoint_exception(){ 
    //clear();
    char* msg = "Error Message: Breakpoint Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void overflow_exception(void)
// This function calls the overflow exception
// Inputs: None
// Outputs: None
void overflow_exception(){ 
    //clear();
    char* msg = "Error Message: Overflow Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}


// void bound_exception(void)
// This function calls the BOUND range exceeded exception
// Inputs: None
// Outputs: None
void bound_exception(){ 
    //clear();
    char* msg = "Error Message: BOUND Range Exceeped Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void opcode_exception(void)
// This function calls the invalid opcode exception
// Inputs: None
// Outputs: None
void opcode_exception(){ 
    //clear();
    char* msg = "Error Message: Invalid Opcode Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void device_not_available(void)
// This function calls the device not available exception
// Inputs: None
// Outputs: None
void device_not_available(){ 
    //clear();
    char* msg = "Error Message: Device Not Available Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void double_fault_exception(void)
// This function calls the double fault exception
// Inputs: None
// Outputs: None
void double_fault_exception(){ 
    //clear();
    char* msg = "Error Message: Double Fault Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void co_segment_overrun(void)
// This function calls the coprocessor segment overrun
// Inputs: None
// Outputs: None
void co_segment_overrun(){ 
    //clear();
    char* msg = "Error Message: Coprocessor Segment Overrun\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void tss_exception(void)
// This function calls the invalid tss exception
// Inputs: None
// Outputs: None
void tss_exception(){ 
    //clear();
    char* msg = "Error Message: Invalid TSS Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void segment_not_present(void)
// This function calls segment not present 
// Inputs: None
// Outputs: None
void segment_not_present(){ 
    //clear();
    char* msg = "Error Message: Segment Not Present\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void stack_fault_exception(void)
// This function calls the stack fault exception
// Inputs: None
// Outputs: None
void stack_fault_exception(){ 
    //clear();
    char* msg = "Error Message: Stack Fault Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void general_protection_exception(void)
// This function calls the general protection exception
// Inputs: None
// Outputs: None
void general_protection_exception(){ 
    //clear();
    char* msg = "Error Message: General Protection Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void page_fault_exception(void)
// This function calls the page fault exception
// Inputs: None
// Outputs: None
void page_fault_exception(){ 
    ////clear();
    char* msg = "Error Message: Page-fault Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void fpu_error(void)
// This function calls the x87 fpu floating-point error
// Inputs: None
// Outputs: None
void fpu_error(){ 
    //clear();
    char* msg = "Error Message: x87 FPU Floating-Point Error\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void alignment_check_exception(void)
// This function calls the alignment check exception
// Inputs: None
// Outputs: None
void alignment_check_exception(){ 
    //clear();
    char* msg = "Error Message: Alignment Check Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void machine_check_exception(void)
// This function calls the machine-check exception
// Inputs: None
// Outputs: None
void machine_check_exception(){ 
    //clear();
    char* msg = "Error Message: Machine-Check Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void simd_fp_exception(void)
// This function calls the simd floating_point exception
// Inputs: None
// Outputs: None
void simd_fp_exception(){ 
    //clear();
    char* msg = "Error Message: SIMD Floating-Point Exception\0";
    printf("%s", msg);
    printf("\n");
    squash((uint8_t)SQUASH_STATUS);
}

// void system_calls_c(void)
// This function occurs when a system call occurs for idt[0x80]
// Inputs: None
// Outputs: None
void system_calls_c(){ 
    //clear();
    //char* msg = " Interrupt Message: System Calls\0";
    //printf("%s", msg);
    system_call_asm();
}

// void keyboard_interrupt(void)
// This function occurs upon a keyboard interrupt
// Inputs: None
// Outputs: None
void keyboard_interrupt(){ // keyboard interrupts
    //printf(" Interrupt Message: Keyboard Interrupt ");
    handle_keyboard();
}

// void rtc_interrupt_c(void)
// This function occurs upon an rtc interrupt
// Inputs: None
// Outputs: None
void rtc_interrupt_c(){ 
    //printf("Interrupt Message: RTC Interrupt");
    rtc_handler();
}

// void pic_interrupt(void)
// This function occurs upon a pic interrupt
// Inputs: None
// Outputs: None
//void pic_interrupt_c(){  
//    printf("Error Message: PIC Interrupt");
//    while(1){
//    }
//}
