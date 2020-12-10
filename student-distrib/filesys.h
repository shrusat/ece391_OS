#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h"
#include "lib.h"
#include "sys_call.h"

//array of directory entries for the file system
//dentry_t* file_dentries;
// array of inodes
//inode_t* inode_array; 
//address of the boot block
//uint32_t boot_block_address; 

#define DATA_SIZE 4096 //size of data
#define LENGTH 32


// helper functions provided by appendix A
//Returns directory entry information from the given name
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
//Returns directory entry information from the given index
int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry);
//Reads bytes starting from 'offset' in the file with the inode 'inode'.
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

// functions that make design align with CP3
int32_t file_read(int32_t fd, void *buf, int32_t nbytes); // fill arguments with something 
int32_t file_write(int32_t fd, const void *buf, int32_t nbytes);
int32_t file_close(int32_t fd);
int32_t file_open(const uint8_t* filename);
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes);
int32_t dir_write(int32_t fd, const void *buf, int32_t nbytes);
int32_t dir_close(int32_t fd);
int32_t dir_open(const uint8_t* filename);

// some variables we might need,,,
uint32_t start_of_file; // starting position of the file 

void init_filesys(const uint32_t start_of_file); 

dentry_t* dentry_array; 
boot_t* boot_block;
inode_t* inode_array;
data_block_t* data_block_array; 
dentry_t file_dentry;
file_descrip_t file_array[8]; // from appendix a 
uint32_t d_index; // used to track directories being read

int32_t get_size(uint32_t inode_number); // used to get the size at a specific inode

//uint32_t * BOOT_BLOCK_PTR;


#endif /* _FILESYS_H */
