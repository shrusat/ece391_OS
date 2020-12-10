// Handles exceptions and device interrupts that are called by the IDT defined in idt.c
// This links to the handler_C file so that the specified error can be printed.
#ifndef _HANDLER_H
#define _HANDLER_H

extern void de_exc(void); // divide error exception

extern void db_exc(void); // debug exception

extern void nmi_int(void); // nmi interrupt

extern void bp_exc(void); // breakpoint exception

extern void of_exc(void); // overflow exception

extern void br_exc(void); // BOUND range exceeded exception

extern void ud_exc(void); // invalid opcode exception

extern void nm_exc(void); // device not available exception

extern void df_exc(void); // double fault exception

extern void cs_overrun(void); // coprocessor segment overrun

extern void ts_exc(void); // invalid TSS exception

extern void np_exc(void); // segment not present

extern void ss_exc(void); // stack fault exception

extern void gp_exc(void); // general protection exception

extern void pf_exc(void); // page-fault exception

extern void mf_exc(void); // x87 FPU floating-point error

extern void ac_exc(void); // alignment check exception

extern void mc_exc(void); // machine-check exception

extern void xf_exc(void); // SIMD floating-point exception

extern void system_calls(void); // IDT[0x80] is reserved for system calls

extern void kb_interrupt(void); // IDT[KEYBOARD_INDEX] interrupt for keyboard

extern void rtc_interrupt(void); // IDT[RTC_INDEX] interrupt for RTC

extern void pit_interrupt(void); // IDT[PIC_INDEX] interrupt for PIT

#endif /* _HANDLER_H */
