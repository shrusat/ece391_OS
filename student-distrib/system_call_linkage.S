#define ASM 1
#include "x86_desc.h"

#define STACK_START 0x083FFFFC 
#define IF_FLAG 0x200

.data

    INVALID = -1 # invalid index in jump table at index 0

.text 

.globl system_call_asm
.global context_switch 
system_call_asm:

    # register save
    # pushal # save registers
    # pushfl # save flags

    /* save all general-use register */
    pushl   %fs
    pushl   %es
    pushl   %ds
    pushl   %ebp
    pushl   %edi
    pushl   %esi

    # system call checking
    cmpl $1, %eax # 1 corresponds to the first system call (halt). 1 is the lowest system call index that can be supported
    jl failed_call
    cmpl $10, %eax # 10 corresponds to the last system call (sigreturn). 10 is the highest system call index that can be supported
    jg failed_call

    pushl %edx # edx is the third arg for system calls (pushed right to left in C)
    pushl %ecx # ecx is the second arg for system calls (pushed right to left in C)
    pushl %ebx # ebx is the first arg for system calls (pushed right to left in C)

    # call correct system call
    call *jump_table(, %eax, 4) # call number stored in eax, 4 because addresses are 4 bytes (32 bits)

    popl %ebx 
    popl %ecx 
    popl %edx 

    # restore registers
    # popfl
    # popal

    /* restore general-use registers*/
    popl    %esi
    popl    %edi
    popl    %ebp
    popl    %ds
    popl    %es
    popl    %fs

    iret

failed_call:

    # restore registers
    # popfl
    # popal

    /* restore general-use registers*/
    popl    %esi
    popl    %edi
    popl    %ebp
    popl    %ds
    popl    %es
    popl    %fs
    
    movl $-1, %eax # -1 value in eax denotes an error

    iret

context_switch:
    # load the entry point into ebx 
    movl 4(%esp), %ebx
    
    # # push SS on to the stack 
    xorl %eax, %eax
    movw $USER_DS , %ax 
    pushl %eax 

    movw $USER_DS, %cx
    movw %cx, %ds 

    # # push esp value for the
    movl $STACK_START, %eax
    pushl %eax 

    # # push eflags 
    pushfl

    # set the IF flag
    popl %eax 
    orl $IF_FLAG , %eax
    pushl %eax 

    # # push the CS on the stack
    pushl $USER_CS

    # # push eip for user program
    pushl %ebx 

    # # switch to user space 

    iret 

jump_table:
	.long INVALID, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

