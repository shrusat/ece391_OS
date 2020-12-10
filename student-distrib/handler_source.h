// Is called by handler.S to deal with exceptions/interrupts. This prints the correct
// error to the screen and freezes the program with an infinite loop.
#ifndef _HANDLER_SOURCE_H
#define _HANDLER_SOURCE_H

#include "keyboard.h"
#include "rtc.h"
#include "system_call_linkage.h"

#define SQUASH_STATUS 256

extern void divide_error_exception(); // c function for divide error exception, called by handler.S assembly linkage

extern void debug_exception(); // c function for debug exception, called by handler.S assembly linkage

extern void nmi_interrupt(); // c function for nmi interrupt, called by handler.S assembly linkage

extern void breakpoint_exception(); // c function for breakpoint exception [same as above]

extern void overflow_exception(); // c function for overflow exception

extern void bound_exception(); // c function for BOUND range exceeded exception

extern void opcode_exception(); // c function for invalid opcode exception

extern void device_not_available(); // c function for device not available exception

extern void double_fault_exception(); // c function for double fault exception

extern void co_segment_overrun(); // c function for coprocessor segment overrun

extern void tss_exception(); // c function for invalid tss exception

extern void segment_not_present(); // c function for segment not present

extern void stack_fault_exception(); // c function for stack fault exception

extern void general_protection_exception(); // c function for general protection exception

extern void page_fault_exception(); // c function for page-fault exception

extern void fpu_error(); // c function for x87 fpu floating_point error

extern void alignment_check_exception(); // c function for alignment check exception

extern void machine_check_exception(); // c function for machine-check exception

extern void simd_fp_exception(); // c function for simd floating-point exception

extern void system_calls_c(); // c function for system calls specified by 0x80

extern void keyboard_interrupt(); // c function for keyboard interrupts

extern void rtc_interrupt_c(); // c function for rtc interrupt

// extern void pic_interrupt_c();  // c function for pic interrupt

#endif /* _HANDLER_SOURCE_H */
