#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"
#include "lib.h"
#include "filesys.h"
#include "rtc.h"
#include "paging.h"
#include "terminal.h"
#include "system_call_linkage.h"

// Constants
#define MAX_OPEN_FILES 8 // each task can have up to 8 open files
#define IN_USE_FLAG 1 // set to 1 if file descriptor is in use
#define NOT_IN_USE_FLAG 0 // set to 0 if file descriptor is not in use
#define FIRST_UNRESERVED_FD 2 // stdin and stdout are reserved as index 0 and 1 in file_array
#define INITIAL_FP 0 // initial file_position when you open a file

#define RTC_FILE 0 // file type is 0 for rtc file
#define DIR_FILE 1 // file type is 1 for dir file
#define REG_FILE 2 // file type is 2 for reg file
#define MAX_SIZE 10
#define SIZE_EXEC 40

#define START_OF_PROGRAM 0x08048000 
#define EIGHTMB 0x800000
#define EIGHTKB 0x2000

#define NULL_CODE '\0'
#define BYTES_4 0x04
#define BIT_MASK_8 8
#define BIT_MASK_16 16
#define BIT_MASK_24 24

#define DIES_BY_EXCEPTION 256
#define _4MB 0x400000 
#define _136MB 0x8800000
#define VIDEO 0xB8000
#define VIDEOBUFFER1 0xB9000
#define VIDEOBUFFER2 0xBA000
#define VIDEOBUFFER3  0xBB000
#define VID_END         0x8400000


// CP3 functions
int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);
void initialize_pcb(uint32_t pid);


int32_t squash(uint8_t status); // squashes programs if they generate exceptions, and returns control to the shell



pcb_t get_pcb(int32_t index);
void increment_file_position(uint32_t pid, int32_t fd, uint32_t increment);
uint32_t get_pid(void);

int32_t bad_call(); // used because we must always call the corresponding open/close/read/write
                        // for a driver, even if it does not do anything

// CP4 functions
int32_t getargs(uint8_t *buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);


#endif /* _SYSCALL_H */


