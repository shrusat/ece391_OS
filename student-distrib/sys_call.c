#include "sys_call.h"
#include "paging.h"




//Global Variables
int32_t pid = 4; // used to index into pcb_arr
int32_t program_count = 0; // used to count number of programs that are currently open
pcb_t pcb_arr[6]; // array of pcb's 
int32_t status_value = 0; 

// Declare file operations jump table 
file_ops_t file_ops_stdin = {terminal_open, terminal_read, bad_call, terminal_close}; // read only corresponding to keyboard input
file_ops_t file_ops_stdout = {terminal_open, bad_call, terminal_write, terminal_close}; // write only corresponding to terminal input
file_ops_t file_ops_RTC = {rtc_open, rtc_read, rtc_write, rtc_close}; // RTC file operations table (should be stored in file operations table pointer when RTC is opened)
file_ops_t file_ops_file = {file_open, file_read, file_write, file_close}; // (regular) file operations table
file_ops_t file_ops_dir = {dir_open, dir_read, dir_write, dir_close}; // directory file operations table

// CP3 functions

/*
* get_pcb()
* This helper function gets the current pcb for filesys functions
* Input: index of pcb_arr
* Output: pcb
*/
pcb_t get_pcb(int32_t index){
    return pcb_arr[index];
}

/*
* increment_file_position()
* This helper function increments the file_position by "increment", given the pcb_arr's index ("pid"),
* and the file descriptor for that pcb (fd)
* Input: pid, fd, and increment
* Output: none
*/
void increment_file_position(uint32_t pid, int32_t fd, uint32_t increment){
    pcb_arr[pid].file_array[fd].file_position += increment;
}

/*
* get_pid()
* This helper function gets the current pid for filesys functions
* Input: none
* Output: pid value
*/
uint32_t get_pid(){
    return pid;
}

/*
* squash()
* This function squashes programs that generate exceptions and returns control to the shell
* Input: status: status of the user program
* Output: This call should never return to the caller
*/
int32_t squash(uint8_t status){
    pcb_t curr_pcb = pcb_arr[pid];
    //check if process exists
    if (&curr_pcb == NULL){
        return -1;
    }

    status = (uint8_t)DIES_BY_EXCEPTION;

    //close any files
    int i;
    for (i = 2; i < 8; i++){ //0 and 1 are stdin and stdout, so start at index 2 until 8
        // initialize_pcb(&(pcb_arr[pid]),pid);?
        curr_pcb.file_array[i].file_flags = NOT_IN_USE_FLAG;
        curr_pcb.file_array[i].file_op_table_ptr = NULL;
        close(i);
    }

    // Multiple terminals
    terminal_t curr_terminal = get_current_terminal();
    for(i = 0; i < MAX_PCBS; i++){ 
        if(curr_terminal.pcb_used[i] == -1){ 
            if(i > 0){ // has a parent, not the first open pcb for that terminal
                //curr_terminal.pcb_used[i-1] = -1; // i - 1 is the index of the last used pcb for this terminal, but since we are halting we need to set it to "not in use"
                update_pcb_used(i-1, -1);
                if((terminal_arr[currTerminal].young_pcb) > 0){
                    (terminal_arr[currTerminal].young_pcb)--;
                }
            }
            break;
        }
    }
    
    //update pid
    
    pcb_arr[pid].occupied = NOT_IN_USE_FLAG;

    pid = pcb_arr[pid].parent_pid;
    program_count--;
   
    //preserve status variable
    status_value = (int32_t)status;

    if(curr_pcb.pid == 0){ //check if last process
        //execute shell
        execute((uint8_t*)("shell"));
    }

    //undo paging
    initialize_program(pid); //pid is now parent_pid
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHTMB - pid*EIGHTKB - BYTES_4; //Change (each get 8 kB)
    
    // restore current esp, ebp 
    asm volatile("          \n\
        movl %0, %%eax      \n\
        movl %1, %%ebp    \n\
        movl %2, %%esp    \n\
        jmp return_execute"
        :
        :"r" (status_value), "r"(curr_pcb.ker_ebp), "r"(curr_pcb.ker_esp)
        : "eax" );

    return 0;
}
/*
* squash()
* This function squashes programs that generate exceptions and returns control to the shell
* Input: status: status of the user program
* Output: This call should never return to the caller
*/
int32_t squash(uint8_t status){
    pcb_t curr_pcb = pcb_arr[pid];
    //check if process exists
    if (&curr_pcb == NULL){
        return -1;
    }

    status = (uint8_t)DIES_BY_EXCEPTION;

    //close any files
    int i;
    for (i = 2; i < 8; i++){ //0 and 1 are stdin and stdout, so start at index 2 until 8
        // initialize_pcb(&(pcb_arr[pid]),pid);?
        curr_pcb.file_array[i].file_flags = NOT_IN_USE_FLAG;
        curr_pcb.file_array[i].file_op_table_ptr = NULL;
        close(i);
    }

    // Multiple terminals
    terminal_t curr_terminal = get_current_terminal();
    for(i = 0; i < MAX_PCBS; i++){ 
        if(curr_terminal.pcb_used[i] == -1){ 
            if(i > 0){ // has a parent, not the first open pcb for that terminal
                //curr_terminal.pcb_used[i-1] = -1; // i - 1 is the index of the last used pcb for this terminal, but since we are halting we need to set it to "not in use"
                update_pcb_used(i-1, -1);
                if((terminal_arr[currTerminal].young_pcb) > 0){
                    (terminal_arr[currTerminal].young_pcb)--;
                }
            }
            break;
        }
    }
    
    //update pid
    
    pcb_arr[pid].occupied = NOT_IN_USE_FLAG;

    pid = pcb_arr[pid].parent_pid;
    program_count--;
   
    //preserve status variable
    status_value = (int32_t)status;

    if(curr_pcb.pid == 0){ //check if last process
        //execute shell
        execute((uint8_t*)("shell"));
    }

    //undo paging
    initialize_program(pid); //pid is now parent_pid
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHTMB - pid*EIGHTKB - BYTES_4; //Change (each get 8 kB)
    
    // restore current esp, ebp 
    asm volatile("          \n\
        movl %0, %%eax      \n\
        movl %1, %%ebp    \n\
        movl %2, %%esp    \n\
        jmp return_execute"
        :
        :"r" (status_value), "r"(curr_pcb.ker_ebp), "r"(curr_pcb.ker_esp)
        : "eax" );

    return 0;
}
/*
* halt()
* This function stops the user programs
* Input: status: status of the user program
* Output: This call should never return to the caller
*/
int32_t halt(uint8_t status){
     //access PCB
    // pcb_t* curr_pcb = (pcb_t*)(EIGHTMB - EIGHTKB*(pid + 1));
    pcb_t curr_pcb = pcb_arr[pid];
    //check if process exists
    if (&curr_pcb == NULL){
        return -1;
    }
    //close any files
    int i;
    for (i = 2; i < 8; i++){ //0 and 1 are stdin and stdout, so start at index 2
        // initialize_pcb(&(pcb_arr[pid]),pid);?
        curr_pcb.file_array[i].file_flags = NOT_IN_USE_FLAG;
        curr_pcb.file_array[i].file_op_table_ptr = NULL;
        close(i);
    }

    // Multiple terminals
    terminal_t curr_terminal = get_current_terminal();
    for(i = 0; i < MAX_PCBS; i++){ 
        if(curr_terminal.pcb_used[i] == -1){ 
            if(i > 0){ // has a parent, not the first open pcb for that terminal
                //curr_terminal.pcb_used[i-1] = -1; // i - 1 is the index of the last used pcb for this terminal, but since we are halting we need to set it to "not in use"
                update_pcb_used(i-1, -1);//last pcb 
                if((terminal_arr[currTerminal].young_pcb) > 0){
                    (terminal_arr[currTerminal].young_pcb)--;
                } 
            }
            break;
        }
    }

    //update pid

    pcb_arr[pid].occupied = NOT_IN_USE_FLAG;

    pid = pcb_arr[pid].parent_pid;
    program_count--;
   
    //preserve status variable
    status_value = (int32_t)status;

    if(curr_pcb.pid == 0){ //check if last process
        //execute shell
        execute((uint8_t*)("shell"));
    }
   
    //undo paging
    initialize_program(pid); //pid is now parent_pid
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHTMB - pid*EIGHTKB - BYTES_4; //Change (each get 8 kB) 

    // restore current esp, ebp 
    asm volatile("          \n\
        movl %0, %%eax      \n\
        movl %1, %%ebp    \n\
        movl %2, %%esp    \n\
        jmp return_execute"
        :
        :"r" (status_value), "r"(curr_pcb.ker_ebp), "r"(curr_pcb.ker_esp)
        : "eax" );



    //return to parent
    // context_switch 
    printf("did not jump to execute.\n");

    return 0;
}


/*
* execute
* This function starts user programs
* Input: command: the command that was typed into the terminal
* Output: [0,255] if program executes a halt system call, 256 if program dies by exception,
* or -1 if command cannot be executed
*/
int32_t execute(const uint8_t* command){
    // if the command is null, abort
    if(command == NULL){
        return -1;
    }

    if(program_count >= 6){ // reaches the max
        return -1;
    }
    
    // Multiple Terminals
    // terminal_t curr_terminal = get_current_terminal();
    int32_t multiple_pid = 0;
    int i;
    terminal_t term = get_current_terminal();
    if(term.pcb_used[0] == -1){
        multiple_pid = term.terminal_number;
    }else{
        for(i = 3; i < MAX_PCBS; i++){ 
            //if(pcb_arr[i].file_array[2].file_op_table_ptr == NULL && i >= program_count){
            //if(pcb_arr[i].file_array[2].file_op_table_ptr == NULL){
            if(pcb_arr[i].occupied == NOT_IN_USE_FLAG){
                multiple_pid = i;
                break;
            }
        }
    }

    // Multiple shells
    // if(0 == strncmp((int8_t*)command,(int8_t*)"shell",5) && (multiple_pid > 2)){ // handle special shell case

    // }
    //     if(program_count > 0){ // multiple shells case
    //         //pid = program_count;  // set pid as normal, do not reset variables
    //         pid = multiple_pid;
    //     //}else{ // single shell case, there are no programs running except for shell
    //     //    pid = 0; // initialize pid to 0
    //     //    program_count = 0; // reset program count to 0
    //     }
    // }else{
    //     //pid = program_count;
    pid = multiple_pid;
   // }

    //PARSE COMMAND
    //uint8_t parse_com[MAX_SIZE];
    //uint8_t arg[MAX_SIZE];
    //int i;
    int com_start = 0; // starting index of command
    int com_end = 0; // ending index of command
    //Parse Command
    //Initializes the buffers
    int k,l;
    for (k = 0; k < MAX_SIZE; k++)
    {
        pcb_arr[pid].parse_com[k] = NULL_CODE;
    }
    for (l = 0; l < MAX_SIZE; l++)
    {
        pcb_arr[pid].arg[l] = NULL_CODE;
    }

    //Checks for spaces
	while (command[com_start] == ' '){ 
        com_start++;
    }	
	com_end = com_start;

    //Finds the ending (space, new line or null)
	while (command[com_end] != ' ' && command[com_end] != NULL_CODE && command[com_end] != '\n'){
		com_end++;
    }

    //Writes into the command buffer
	for (i = com_start; i < com_end; i++){
        pcb_arr[pid].parse_com[i-com_start] = command[i];
    }
	pcb_arr[pid].parse_com[com_end] = NULL_CODE;

    //Writes into the argument buffer
    int j;
    com_end++; // skip the space between command and arg

    for(j=0; j < MAX_SIZE; j++){
        if(com_end >= strlen((int8_t *)command)){
           pcb_arr[pid].arg[j] = NULL_CODE; 
        }
        else{
            pcb_arr[pid].arg[j] = command[com_end];
            com_end++;
        }
    }

    //EXECUTABLE CHECK
    //Check command is in fsdir
    //Read dentry
    //Read data -> read first 40 bytes (4 = 0x7F ELF(Ascii)) (Next 36 is important)

    dentry_t dentry;
    uint8_t buf[SIZE_EXEC];

    if(-1 == read_dentry_by_name(pcb_arr[pid].parse_com, &dentry)){
        return -1;
    }
    if(read_data(dentry.inode_number,0,buf,SIZE_EXEC) < SIZE_EXEC){
        return -1;
    }

    if(!((buf[0] == 0x7F) && (buf[1] == 0x45) && (buf[2] == 0x4C) && (buf[3] == 0x46))){ //This are the first for bytes for executables (Executable: 0x75 ELF)
        return -1;
    }

    //Find the entry address
    uint32_t entry; 
    entry = (buf[27] << BIT_MASK_24)+(buf[26] << BIT_MASK_16)+(buf[25] << BIT_MASK_8)+(buf[24]); // The 24-27th index has the address (Little endian) 
    //entry = 0x080482E8; -> for shell

    //paging
    initialize_program(pid); // need to declare pid somewhwere 
    
    // user level program loader
    read_data(dentry.inode_number,0,(uint8_t*)START_OF_PROGRAM,get_size(dentry.inode_number)); // need to figure out size of buffer,, we r using the kernel stack,, kernel stack is size 8kb
    
     //create/set up pcb
    initialize_pcb(pid);

    //CONTEXT SWITCH
    program_count++; // every time you initialize a new pcb, increment the program count

    //Update TSS also in halt change val in same format
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHT_MB - (pid)*EIGHTKB - BYTES_4; //Change (each get 8 kB)

    // if(program_count == 1 || program_count == 2){ //check if last process
    //     //execute shell
    //     execute((uint8_t*)("shell"));
    // }

    asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1;"  
        :"=r"(pcb_arr[pid].ker_ebp), "=r"(pcb_arr[pid].ker_esp));

    context_switch(entry); // this does all the context switching

    asm volatile( 
        "return_execute:");

    //return status from halt
    // int return_status = status_value;
    // status_value = 0; //reset status
    // return return_status;

    return 0;
}

/*
* void initialize_pcb(uint32_t pid)
* This function is a helper function that initializes the pcb
* Input: pid: program id
* Output: none
* Side Effect: initializes the pcb struct at that pid
*/
void initialize_pcb(uint32_t pid){
    int i;
    for(i=0 ; i < 8; i++){
       // why will it page fault = could be any random value -- region that hasnt be allocated 
        pcb_arr[pid].file_array[i].file_op_table_ptr = NULL; 
        pcb_arr[pid].file_array[i].file_inode = 0; // initialize file_inode, position, flags to 0
        pcb_arr[pid].file_array[i].file_position = 0;
        pcb_arr[pid].file_array[i].file_flags = NOT_IN_USE_FLAG;
    }
    //intialize stdin 
    pcb_arr[pid].file_array[0].file_op_table_ptr = &file_ops_stdin; // ampersand -- pointer to the structure 
    pcb_arr[pid].file_array[0].file_inode = 0; // initialize file_inode, position to 0
    pcb_arr[pid].file_array[0].file_position = 0;
    pcb_arr[pid].file_array[0].file_flags = IN_USE_FLAG;

    //intialize stdout 
    pcb_arr[pid].file_array[1].file_op_table_ptr = &file_ops_stdout; // pounter to the structure 
    pcb_arr[pid].file_array[1].file_inode = 0; // initialize file_inode, position to 0
    pcb_arr[pid].file_array[1].file_position = 0;
    pcb_arr[pid].file_array[1].file_flags = IN_USE_FLAG;

    //just call open 

    //intialize the pid 
    pcb_arr[pid].pid = pid; 

    // parents calls execute ,, pid of the parrent ,, needs to go out w the child pid 
    //if(pid == 0){ 
    //    pcb_arr[pid].parent_pid = -1; // the parent pid of pid 0 does not exist. Use -1 to show it is invalid
    //}else{

    // Multiple terminals - must update pcb_used array and parent_pid
    pcb_arr[pid].occupied = IN_USE_FLAG;
    uint32_t parent = 0;

    if(pid == 0 || pid == 1 || pid == 2){ // terminal 1, 2, and 3
        update_initial_pcb(pid); // initial terminals will always be the first index in pcb_used, where pid number's and terminal indexes are the same
        parent = -1; // first index in pcb_used will never have a parent pid since there is nothing before it
    }else{
        terminal_t curr_terminal = get_current_terminal();
        for(i = 1; i < MAX_PCBS; i++){ 
            if(curr_terminal.pcb_used[i] == -1){ // get first unused pcb for current terminal
                update_pcb_used(i, pid); // set pid to it's respective terminal
                if((terminal_arr[currTerminal].young_pcb) < 6){
                    (terminal_arr[currTerminal].young_pcb)++;
                }
                // if(i == 0){ // if i is 0, there is no parent pid since it is the first element of pcb_used
                //     parent = -1; // -1 shows that parent is invalid
                // }
                // else{
                parent = curr_terminal.pcb_used[i - 1];
                //}
                break;
            }
        }
    }
    pcb_arr[pid].parent_pid = parent; // the parent pid of the current pid is dependent on the current terminal
    //}

    //intialize esp, ebp to 0
    pcb_arr[pid].ker_ebp = 0;
    pcb_arr[pid].ker_esp = 0;

    // save current esp, ebp 
    asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1;"  
        :"=r"(pcb_arr[pid].ker_ebp), "=r"(pcb_arr[pid].ker_esp));
}

/*
* read
* This function reads data from the keyboard, a file, RTC, or directory.
* Input: file descriptor, ptr to buffer, number of bytes to read
* Output: number of bytes read, -1 on failure
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes){
    // Check if initial file position is at or beyond end of file
    if( (fd >= MAX_OPEN_FILES) || (fd < 0) ){ // not in bounds of file array -> indices 0 to 7 are valid
        return -1;
    }
    if(buf == NULL){
        return -1;
    }
    if(pcb_arr[pid].file_array[fd].file_flags == NOT_IN_USE_FLAG){ 
        return -1;
    }
    if(pcb_arr[pid].file_array[fd].file_flags == IN_USE_FLAG){
        return pcb_arr[pid].file_array[fd].file_op_table_ptr->read(fd, buf, nbytes);
    }
    return -1; // nothing was read
}

/*
* write
* This function writes data from the keyboard, a file, RTC, or directory.
* Input: file descriptor, ptr to buffer, number of bytes to write
* Output: number of bytes written, -1 on failure
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
    if(fd >= MAX_OPEN_FILES || fd < 0){ // not in bounds of file array -> indices 0 to 7 are valid
        return -1;
    }
    if(buf == NULL){
        return -1;
    }
    if(pcb_arr[pid].file_array[fd].file_flags == NOT_IN_USE_FLAG){
        return -1;
    }
    if(pcb_arr[pid].file_array[fd].file_flags == IN_USE_FLAG){
        return pcb_arr[pid].file_array[fd].file_op_table_ptr->write(fd, buf, nbytes);
    }else{
        return 0; // fd is not in use, so technically wrote 0 bytes
    }
    
    return 0; // nothing was written

}

/*
* open
* This function opens the file
* Input: filename: file name to open
* Output: 0 on success, -1 on failure
*/
int32_t open(const uint8_t* filename){
    if(filename == NULL || filename[0] == '\0'){ // empty string or null pointer
        return -1; // documentation says -1 return value is failure
    }
    dentry_t dentry;
    uint32_t current_file_type;
    int i; // used to iterate through file_array

    int32_t result = read_dentry_by_name(filename, &dentry);
    if(result == -1){ // -1 return value corresponds to the dentry not being in the system
        return -1; // -1 means failure (in documentation) since nothing to open 
    }
    current_file_type = dentry.file_type; // holds whether it is dir, regular file, or rtc file

    for(i = FIRST_UNRESERVED_FD; i < MAX_OPEN_FILES; i++){ // starts at index 2
        if(pcb_arr[pid].file_array[i].file_flags == NOT_IN_USE_FLAG){ // find first unused file_descriptor
            pcb_arr[pid].file_array[i].file_flags = IN_USE_FLAG; // update flag
            pcb_arr[pid].file_array[i].file_position = INITIAL_FP; // initially must read from beginning of file

            // check what file type it is
            if(current_file_type == RTC_FILE){
                pcb_arr[pid].file_array[i].file_inode = 0; // inode is 0 for rtc files
                pcb_arr[pid].file_array[i].file_op_table_ptr = (&file_ops_RTC);
                return i;
            }else if(current_file_type == DIR_FILE){
                pcb_arr[pid].file_array[i].file_inode = 0; // inode is 0 for directories
                pcb_arr[pid].file_array[i].file_op_table_ptr = (&file_ops_dir);
                return i;
            }else if(current_file_type == REG_FILE){
                pcb_arr[pid].file_array[i].file_inode = (uint32_t)dentry.inode_number; // set to corresponding inode number
                pcb_arr[pid].file_array[i].file_op_table_ptr = (&file_ops_file);
                return i;
            }else{
                return -1; // there is no valid file to open
            }
            //break;
        }
    }
    return -1; // end of file_array, nothing to open

}

/*
* close
* This function undoes what was done in open
* Input: fd: file dentry
* Output: 0 on success, -1 on failure
*/
int32_t close(int32_t fd){
    if(fd == 0 || fd == 1){ // 0 and 1 correspond to STDIN and STDOUT, which cannot be closed
        return -1; // -1 is what we return on failure
    }
    if(fd > 7 || fd < 0){ // index of file array only goes from 0 to 7
        return -1; // -1 is what we return on failure
    }
    // int32_t result;
    // result = pcb_arr[0].file_array[fd].file_op_table_ptr->close(fd);
    if(pcb_arr[pid].file_array[fd].file_flags == NOT_IN_USE_FLAG){
        return -1;
    }
    if(pcb_arr[pid].file_array[fd].file_flags == IN_USE_FLAG){
        pcb_arr[pid].file_array[fd].file_flags = NOT_IN_USE_FLAG;
        pcb_arr[pid].file_array[fd].file_inode = 0; // reset to 0 since no longer in use
        pcb_arr[pid].file_array[fd].file_position = 0; // reset to 0 since no longer in use
        pcb_arr[pid].file_array[fd].file_op_table_ptr = NULL; // reset to null since no longer in use
        return 0;
    }

    return -1; // failed close

}

/*
* int32_t bad_call()
* This function returns -1 if failed
* Input: none
* Output: -1 on failure
*/
int32_t bad_call(){
    return -1;
}

// CP4 functions

/*
* getargs
* This function gets the arguments from execute
* Input: buf: buf for arguments, nbytes: number of bytes read
* Output: 0 on success, -1 on failure
*/
int32_t getargs(uint8_t *buf, int32_t nbytes){
    int i;
    if(buf == NULL){ 
        return -1;
    }
    if(pcb_arr[pid].arg[0] == '\0'){ // when a command has no argument (ie: 'grep ' vs. 'grep /\')
        return -1;
    }
    for(i = 0; i < nbytes; i++){
        buf[i] = NULL_CODE;
    }
    for(i = 0; i < nbytes; i++){
        buf[i] = pcb_arr[pid].arg[i];
    }
    return 0;
}




/*
* vidmap
* This function displays video memory on the screen
* Input: screen_start: where user video memory starts
* Output: 0 on success, -1 on failure
*/
int32_t vidmap(uint8_t** screen_start){
    if (screen_start == NULL)
	{
		return -1;
	}
    if (screen_start == (uint8_t**)_4MB){
        return -1;
    }
    //remaps the physical address to the video memory or buffer
    if(currTerminal == runTerminal){
        remap_page((uint32_t)_136MB, (uint32_t)VIDEO); 
    }
    else if(runTerminal == 0){
        remap_page((uint32_t)_136MB, (uint32_t)VIDEOBUFFER1); 
    }
    else if(runTerminal == 1){
        remap_page((uint32_t)_136MB, (uint32_t)VIDEOBUFFER2); 
    }
    else if(runTerminal == 2){
        remap_page((uint32_t)_136MB, (uint32_t)VIDEOBUFFER3); 
    }
    
     // 136 mb since that is greater than 132 ,, b8000 for start of video mem
    *screen_start = (uint8_t*)_136MB; // 0x1400000 //x84 // physical mem address of the user programs video memory
    return 0; 

    //either background or screen 
}

/*
* set_handler()
* This function ...
* Input: 
* Output: 0 on success, -1 on failure
*/
int32_t set_handler(int32_t signum, void* handler_address){
    return bad_call();
}

/*
* sigreturn()
* This function ...
* Input: 
* Output: 0 on success, -1 on failure
*/
int32_t sigreturn(void){
    return bad_call();
}

