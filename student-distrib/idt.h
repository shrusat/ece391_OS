# ifndef _IDT_H
# define _IDT_H

# define SYSTEM_CALL_INDEX 0x80 // 0x80 is reserved for system calls
# define SYSTEM_CALL_PRIVILEGE 0x3 // privilege level is set to 3 for system calls

#define KEYBOARD_INDEX 0x21 // 0x21 is the index for keyboard
#define RTC_INDEX 0x28 // 0x28 is the index for RTC
#define PIT_INDEX 0x20 // 0x20 is the PIT index

// This function initializes the IDT
void init_idt(void);

#endif /* _IDT_H */
