.text
# Handles exceptions and device interrupts that are called by the IDT defined in idt.c
# Links to handler_C so that the proper print function can be called.

 #define ASM     1

# void de_exc(void);
# Calls divide_error_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl de_exc
de_exc:
    pushal # save registers
    pushfl # save flags
    call divide_error_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void db_exc(void);
# Calls debug_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl db_exc
db_exc:
    pushal # save registers
    pushfl # save flags
    call debug_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void nmi_int(void);
# Calls nmi_interrupt from handler_C.h
# Inputs: none
# Outputs: none
.globl nmi_int
nmi_int:
    pushal # save registers
    pushfl # save flags
    call nmi_interrupt
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void bp_exc(void);
# Calls breakpoint_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl bp_exc
bp_exc:
    pushal # save registers
    pushfl # save flags
    call breakpoint_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void of_exc(void);
# Calls overflow_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl of_exc
of_exc:
    pushal # save registers
    pushfl # save flags
    call overflow_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void br_exc(void);
# Calls bound_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl br_exc
br_exc:
    pushal # save registers
    pushfl # save flags
    call bound_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void ud_exc(void);
# Calls opcode_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl ud_exc
ud_exc:
    pushal # save registers
    pushfl # save flags
    call opcode_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void nm_exc(void);
# Calls device_not_available from handler_C.h
# Inputs: none
# Outputs: none
.globl nm_exc
nm_exc:
    pushal # save registers
    pushfl # save flags
    call device_not_available
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void df_exc(void);
# Calls double_fault_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl df_exc
df_exc:
    pushal # save registers
    pushfl # save flags
    call  double_fault_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void cs_overrun(void);
# Calls co_segment_overrun from handler_C.h
# Inputs: none
# Outputs: none
.globl cs_overrun
cs_overrun:
    pushal # save registers
    pushfl # save flags
    call co_segment_overrun
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void ts_exc(void);
# Calls tss_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl ts_exc
ts_exc:
    pushal # save registers
    pushfl # save flags
    call tss_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void np_exc(void);
# Calls segment_not_present from handler_C.h
# Inputs: none
# Outputs: none
.globl np_exc
np_exc:
    pushal # save registers
    pushfl # save flags
    call segment_not_present
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void ss_exc(void);
# Calls stack_fault_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl ss_exc
ss_exc:
    pushal # save registers
    pushfl # save flags
    call stack_fault_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void gp_exc(void);
# Calls general_protection_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl gp_exc
gp_exc:
    pushal # save registers
    pushfl # save flags
    call general_protection_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void pf_exc(void);
# Calls page_fault_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl pf_exc
pf_exc:
    pushal # save registers
    pushfl # save flags
    call page_fault_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void mf_exc(void);
# Calls fpu_error from handler_C.h
# Inputs: none
# Outputs: none
.globl mf_exc
mf_exc:
    pushal # save registers
    pushfl # save flags
    call fpu_error
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void ac_exc(void);
# Calls alignment_check_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl ac_exc
ac_exc:
    pushal # save registers
    pushfl # save flags
    call alignment_check_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void mc_exc(void);
# Calls machine_check_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl mc_exc
mc_exc:
    pushal # save registers
    pushfl # save flags
    call machine_check_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void xf_exc(void);
# Calls simd_fp_exception from handler_C.h
# Inputs: none
# Outputs: none
.globl xf_exc
xf_exc:
    pushal # save registers
    pushfl # save flags
    call simd_fp_exception
    popfl # restore flags
    popal # restore flags

    leave
    ret

# void system_calls(void);
# Calls system_calls_c from handler_C.h
# Inputs: none
# Outputs: none
.globl system_calls
system_calls:
    pushal # save registers
    pushfl # save flags
    call system_calls_c
    popfl # restore flags
    popal # restore flags

    iret

# void kb_interrupt(void);
# Calls keyboard_interrupt from handler_C.h
# Inputs: none
# Outputs: none
.globl kb_interrupt
kb_interrupt:
    pushal # save registers
    pushfl # save flags
    call keyboard_interrupt
    popfl # restore flags
    popal # restore flags

    iret

# void rtc_interrupt(void);
# Calls rtc_interrupt_c from handler_C.h
# Inputs: none
# Outputs: none
.globl rtc_interrupt
rtc_interrupt:
    pushal # save registers
    pushfl # save flags
    call rtc_interrupt_c
    popfl # restore flags
    popal # restore flags

    iret

# void pit_interrupt(void);
# Calls pit_interrupt_c from handler_C.h
# Inputs: none
# Outputs: none
.globl pit_interrupt
pit_interrupt:
    pushal # save registers
    pushfl # save flags
    call pit_handler
    popfl # restore flags
    popal # restore flags

    iret
