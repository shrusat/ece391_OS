/*
rtc.h - Defines functions to handle the rtc
*/
#ifndef _RTC_H
#define _RTC_H

#include "i8259.h"
#include "types.h"
#include "lib.h"

// added return value to read/write

//magic numbers
#define RTC_INDEX_1  0x70 
#define RTC_RW     0x71
#define RA         0x0A //register A
#define RB         0x0B //register B
#define RC        0x0C //register C
#define RTC_IRQ   0x08
#define DISABLE_A 0x8A // the way i got this number is that to disable nmi u need to set the x80 bit and A is 0x0A
#define DISABLE_B 0x8B // same way i got A
#define DISABLE_C 0x8C // same way i got A
#define DEFAULT_FREQ    2 // default frequency of RTC is 2Hz

#define RATE_2  15
#define RATE_4  14
#define RATE_8  13
#define RATE_16  12
#define RATE_32  11
#define RATE_64  10
#define RATE_128  9
#define RATE_256  8
#define RATE_512  7
#define RATE_1024  6
// Initializes rtc
void initialize_rtc(void);

// handles interrupts
void rtc_handler(void);

void change_frequency(uint32_t frequency);

int32_t rtc_open(const uint8_t * filename);

int32_t rtc_close(int32_t fd);

int32_t rtc_read(int32_t fd, void * buf, int32_t n_bytes);

int32_t rtc_write(int32_t fd, const void * buf, int32_t n_bytes);

#endif
