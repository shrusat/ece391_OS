#ifndef _SYSTEM_CALL_LINKAGE_H
#define _SYSTEM_CALL_LINKAGE_H

#include "lib.h"

extern void system_call_asm(void); // system call assembly linkage
extern void context_switch(uint32_t entry); // context switch 

#endif /* _SYSTEM_CALL_LINKAGE_H */