#include "filesys.h"
#include "x86_desc.h"
#include "lib.h"

//dentry_t* dentry_array; 
//boot_t* boot_block;
//inode_t* inode_array;
//data_block_t* data_block_array; 
//dentry_t file_dentry;
//file_descrip_t file_array[8]; // from appendix a 
/*
* init_filesys
* This function fills in the dentry_t block passed in with file name, file type, and inode number for the file
* Inputs:   start of file
* Outputs:  void so not output
*/
void init_filesys(const uint32_t start_of_file){
    boot_block = (boot_t*) start_of_file;
    dentry_array = (dentry_t*) (start_of_file + 64); // 64 b dir entries
    inode_array = (inode_t*) (start_of_file + DATA_SIZE); // you want to skip the boot block, and every block is 4kb

    // ask where data block is located 
    data_block_array = (data_block_t*) (start_of_file + (DATA_SIZE + DATA_SIZE*(boot_block->inodes_n)));
}

/*
* read_dentry_by_name
* This function fills in the dentry_t block passed in with file name, file type, and inode number for the file
* Inputs:   ptr to fname, ptr to dentry
* Outputs:  Returns 0 on success
*           Returns -1 on failure (non-existent file or invalid index)
*/
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){

    // check for invalid fname or dentry, return -1 on failure
    if(dentry == NULL || fname == NULL){
        return -1;
    }

    int32_t i;
    //int8_t* fname_length = (int8_t*)strlen(fname);
   // int8_t* dentry_name_length;

    // iterate through each dentry
    for(i = 0; i < MAX_DIR_ENTRIES; i++){
        // check if strings are the same length
        //dentry_name_length = (int8_t*)strlen(dentry_array[i].file_name);
        //if(dentry_name_length != fname_length){
           // break; // invalid, return -1
       // }
        // check if dentry name = fname,
        if(strncmp((int8_t*)fname, (int8_t*)boot_block->entries_dir[i].file_name, FILENAME_LENGTH) == 0 && strlen((int8_t*)fname)<= LENGTH){
            // if it does, fill dentry
            strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->entries_dir[i].file_name,FILENAME_LENGTH);
            dentry->file_type = boot_block->entries_dir[i].file_type;
            dentry->inode_number = boot_block->entries_dir[i].inode_number;
            return 0;
    }
    }
    // otherwise, return -1
    return -1;


}

/*
* read_dentry_by_index
* This function fills in the dentry_t block passed in with file name, file type, and inode number for the file
* Inputs:   ptr to index, ptr to dentry
* Outputs:  Returns 0 on success
*           Returns -1 on failure (non-existent file or invalid index)
*/
int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry){
   if(d_index >= boot_block->dir_entries){
        d_index = 0; // if d_index has read all dentries, reset global d_index to read from index 0
                     // this reset allows ls to occur multiple times
   }
   if(index >= boot_block->dir_entries){
        return -1; // invalid index
   }
   // if(dentry_array[index] == NULL){
   //     return -1; // non-existent file
   // }

    // fill in dentry_t block since index is valid
    strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->entries_dir[index].file_name, FILENAME_LENGTH);
    dentry->file_type = boot_block->entries_dir[index].file_type;
    dentry->inode_number = boot_block->entries_dir[index].inode_number;

    return 0; 

}


/*
* read_data
* Reads up to 'length' bytes starting from 'offset' in the file with 'inode'
* Checks that a given inode is within the valid range
* Inputs:   inode, offset, ptr to buffer, length
* Outputs:  Returns number of bytes read and placed in buffer on success
*           Returns -1 on failure (invalid inode number or bad data block number is found within file bounds)
*           Returns 0 if end of file has been reached
*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length){
    uint32_t i;
   int32_t number_of_bytes = 0;
   uint32_t curr_data_block; // data to copy to buf
   int32_t off_data_block; // data block number in inode
   int32_t new_offset; // offset once inside correct data block
   inode_t *curr_inode;
   if(inode < 0 || inode >= boot_block->inodes_n){ // anything with inode number less than 0 or greater than/equal to the 
                                                   // number of inodes is invalid and should return 0
       return number_of_bytes;
   }

   new_offset = offset%DATA_SIZE;
   off_data_block = offset/DATA_SIZE;
   curr_inode = (inode_t*)((uint8_t*)boot_block+((inode+1)*DATA_SIZE));

   if(offset >= curr_inode->size) // you are at end of file, there is nothing more to read so return 0
   {
        return number_of_bytes; 
   }
  // curr_data_block = (data_block_t*)((uint8_t*)boot_block+(1+boot_block->inodes_n+curr_inode->data_block_num[off_data_block])*4096);
   curr_data_block = (uint32_t)((uint8_t*)boot_block+(1+64+curr_inode->data_block_num[off_data_block])*DATA_SIZE); // deleted data block bc its set already in mem and might mess it up if i cast it
    // 64 was a set number from discussion
   // copy to buffer
   for(i=0;i<length;i++){
       if(number_of_bytes+offset < curr_inode->size){ // less than the length within inode
           buf[i] = *((uint32_t*)(curr_data_block+new_offset));
           number_of_bytes++;
           new_offset++;
            // move to next data block
           if(new_offset >= DATA_SIZE){ // 4(1024)
                off_data_block++;
                curr_data_block = (uint32_t)((uint8_t*)boot_block+(1+64+curr_inode->data_block_num[off_data_block])*DATA_SIZE); // 64 was a set number from discussion
                new_offset = 0; // 0 is the file position of the next file
           }
       }
       else{
            //return 0;
           break;
       }
   }
   return number_of_bytes;

}

/*
* file_read 
* Reads count bytes of data from file into buf (uses read_data)
* Inputs: file descriptor, buffer to fill, number of bytes to read
* Outputs: -1 on failure, number of bytes read on success
*/
int32_t file_read(int32_t fd, void *buf, int32_t nbytes){
    if(fd < 0 || fd > 7){ // fd's less than 0 and greater than 7 are not valid and should return -1
        return -1;
    }
    //memset(buf,NULL,sizeof(buf));

    pcb_t pcb;
    uint32_t index;
    uint32_t pid;
    uint32_t bytes_read;
    pid = get_pid(); // get the current pid 
    pcb = get_pcb(pid); // get the pcb from pcb_arr[pid] using helper
    index = pcb.file_array[fd].file_inode; // grab the inode at fd for the pcb we just accessed
    uint32_t data_offset = pcb.file_array[fd].file_position; // grab that same pcb's file_position to know where to start read from 
            
    bytes_read = read_data(index,data_offset,buf,nbytes); 
    increment_file_position(pid, fd, bytes_read); // use helper to update file position for the file we just read from
    return bytes_read; 
} 
 

/*
* file_write
* Does nothing
* Inputs: Nothing
* Outputs: Returns -1
*/
int32_t file_write(int32_t fd, const void *buf, int32_t nbytes){
    return -1;
}

/*
* file_close
* Undos what you did in the open function
* Inputs:
* Outputs: Return -1
*/
int32_t file_close(int32_t fd){
    return 0;
}

/*
* file_open
* Initializes any temporary structures
* Inputs: filename
* Outputs: Return 0
*/
int32_t file_open(const uint8_t* filename){
    return (read_dentry_by_name(filename, &file_dentry));
}

/*
* dir_read
* Read files by filename, including "."
* Inputs: Reads files from the directory by filename
* Outputs: 
*/
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes){
    dentry_t dentry;
    uint32_t size;

    if(read_dentry_by_index(d_index, &dentry) == 0){
        strncpy((int8_t*)buf, (int8_t*)dentry.file_name, (uint32_t)nbytes);
    }
    else{
        return 0; // at or beyond end of file
    }

    d_index++; // subsequent reads should read from successive dentries
    size = strlen((int8_t*)dentry.file_name); // cannot read more than string length or 32 bytes
    if(size > FILENAME_LENGTH){ // 32 is max string length, so you can't read more than that
        return FILENAME_LENGTH; 
    }else{
        return size; // return size of what was read, if size is less than the max filename length
    }
    
}

/*
* dir_write
* Do nothing
* Inputs: Nothing
* Outputs: Return -1
*/
int32_t dir_write(int32_t fd, const void *buf, int32_t nbytes){
    return -1;
}

/*
* dir_close
* [Probably] does nothing
* Inputs: Nothing
* Outputs: Return 0
*/
int32_t dir_close(int32_t fd){
    return 0;
}

/*
* dir_open 
* Opens a directory file (note file types)
* Inputs: filename
* Outputs: Return 0
*/
int32_t dir_open(const uint8_t* filename){
 return (read_dentry_by_name(filename, &file_dentry));
}

/*
* get_size 
* Helper that gets the size of an inode, given an inode number
* Inputs: inode_number
* Outputs: Return size of inode
*/
int32_t get_size(uint32_t inode_number){
    return inode_array[inode_number].size;
}

