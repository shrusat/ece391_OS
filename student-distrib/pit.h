#ifndef _PIT_H
#define _PIT_H

#include "i8259.h"
#include "types.h"
#include "lib.h"
#include "scheduler.h"

// Constants, port numbers, and information on how to set up PIT found on:
// https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interval_Timer
// PIT Mode information found on: https://wiki.osdev.org/Programmable_Interval_Timer

#define PIT_IRQ 0 // IRQ # = 0 corresponds to timer chip 

#define PIT_FREQ 50 // MP3 documentation says we want an interrupt every 10-50 ms. 20 ms = 50 Hz
                     // frequency = 1/time = 1/20ms = 1/0.02s = 50 Hz

#define PIT_DIVISOR 23864 // PIT has oscillator that emits signal at 1193182 Hz. If you want an interrupt at 
                          // certain frequency f (50 Hz), you need the divisor which is equal to 
                          // 1193182 Hz / f -> 1193182 Hz / 50 Hz = 23864

#define PIT_CHANNEL_0 0x40 // channel 0 is used as system timer

#define PIT_COMMAND 0x43 // Command port for PIT

#define PIT_MODE 0x36 // 00 11 011 0 (explanation below)
                      // [7:6] - Channel 0 -> 00 (channel 0 is used for IRQ0 timer interrupts)
                      // [5:4] - Access mode: lobyte/hibye -> 11 (16 bits will be transferred SEQUENTIALLY)
                      // [3:1] - Mode 3 -> 011 (usually Mode 3 is used to generate IRQ0 interrupts)
                      // [0] - 16-bit binary -> 0 (x86 uses binary)

#define LOW_MASK 0x00FF // used to mask low 8 bits of divisor
#define HIGH_MASK 0xFF00 // used to mask high 8 bits of divisor

void initialize_pit(void); // initializes pit (called in kernel.c)

void pit_handler(void); // handles the pit interrupt

#endif
