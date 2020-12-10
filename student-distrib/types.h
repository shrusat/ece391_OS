/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0

#ifndef ASM

/* Types defined here just like in <stdint.h> */
typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

// Constants
#define ARR_SIZE 24
#define ARG_SIZE 10
#define FILE_SIZE 8
#define FILENAME_LENGTH 32 // each dentry has a name up to 32 characters
#define MAX_DIR_ENTRIES 63 // file system can hold up to 63 files (4kB blocks / 64 bytes per dentry = 63 files)
#define MAX_PCBS 6
#define MAX_BUFF 128
#define MAX_SCREEN 2000 //80 * 25

// all the structs 
typedef struct dentry{
   uint8_t file_name[FILENAME_LENGTH];
   uint32_t file_type;
   uint32_t inode_number;
   uint8_t reserved[ARR_SIZE]; 
} dentry_t; 

/*
* The boot block holds both file system stats and dentries.
* Stats and each directory entry occupy 64 bytes
* File system can hold up to 63 files
* First directory can only hold 62 files since it refers to directory itself
*/


typedef struct boot {
    uint32_t dir_entries;
    uint32_t inodes_n;
    uint32_t data_n;
    uint8_t reserved[52];
    dentry_t entries_dir[MAX_DIR_ENTRIES];
} boot_t;

/*
* Each data block contains 4kB and make up each regular file
*/
typedef struct data_block{
    uint8_t data_size[4096]; // 4kb 
} data_block_t;
// File Operations Jump Table Struct
// used to call specific open/read/write/close for each device
typedef struct file_ops{
    int32_t (*open)(const uint8_t* filename);
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*close)(int32_t fd);
} file_ops_t;

typedef struct file_descrip{
    struct file_ops *file_op_table_ptr; 
   // file_ops_t *file_op_table_ptr;
    uint32_t file_inode; 
    uint32_t file_position; 
    uint32_t file_flags; 
} file_descrip_t;

typedef struct pcb{
    //struct file_descrip ;
    struct file_descrip file_array[FILE_SIZE]; // file array
    uint32_t parent_pid;
    uint32_t pid;
    uint32_t cr3;
    uint32_t ker_esp;
    uint32_t ker_ebp;
    uint8_t parse_com[ARG_SIZE];
    uint8_t arg[ARG_SIZE];
    uint32_t tss; // for the switching stuff 
    uint32_t occupied;
    uint32_t flag;
}pcb_t;

typedef struct terminal{
    uint32_t pcb_used[MAX_PCBS]; //Max pcb in the terminal
    int young_pcb;
    int terminal_number; // terminal 0, 1, and 2
    int curr_buff;
    int cursor_x;
    int cursor_y;
    pcb_t* curr_pcb;
}terminal_t;



/*
* Each regular file has:
*   inode (specifies file size in bytes)
*   data blocks 
*/
typedef struct inode{
    uint32_t size;
    uint32_t data_block_num[1023]; // 4096 bytes/4 bytes - 1
                                   // We subtract 1 because the first entry in an inode holds file size
}inode_t;

#endif /* ASM */

#endif /* _TYPES_H */

