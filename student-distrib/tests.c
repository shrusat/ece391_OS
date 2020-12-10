#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "keyboard.h"
#include "terminal.h"
#include "filesys.h"
#include "sys_call.h"


#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */

int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here
// non existant page check
void non_existant_page(){
	TEST_HEADER;
	int* ptr = (int*)(0x800000 + 5);
	int test;
	test = *(ptr);
}

//check video memory is in range
int check_video_memory(){
	TEST_HEADER;
	int* ptr = (int*)(0xB8000 + 5);
	int test;
	test = *(ptr);
	return PASS;
}
// check if kernel is in range 
int check_kernel(){
	TEST_HEADER;
	int* ptr = (int*)(0x400000 + 5);
	int test;
	test = *(ptr);
	return PASS;
}
//null test for paging
void null_paging(){
	TEST_HEADER;

	int* ptr = NULL;
	int test;
	test = *(ptr);
}
void idt_divide_error(){
    TEST_HEADER;
    int i = 0;
    int j = 1;
    i = j/i;
}
void idt_page_fault_exception(){
   TEST_HEADER;
   int y = 0;
   int *x = NULL;
   y = *(x);
}



/* Checkpoint 2 tests */
// print file
// going to make a print buffer helper function
int print_buffer(uint8_t* buf, uint32_t length){
    int32_t i;
    for (i = 0; i < length; i++) {
        if(buf[i] != '\0'){
      putc(buf[i]); // make a separate counter // check null characters 
        }
    }
    printf("this is the length %d:\n", length);
    return 0;
}

void print_file_name(){
   TEST_HEADER;
   dentry_t* dentry;
   uint32_t inode_num;
    clear();
    printf("\n");

    if(read_dentry_by_name((const uint8_t*)"frame0.txt", dentry) == 0){ // 0 means function call was a success
        printf(" File Name: ");
        printf("%s", (uint8_t*)dentry->file_name);
        printf(" File Type: ");
        printf("%u", (uint32_t*)dentry->file_type);
        printf(" Inode: ");
        printf("%u", (uint32_t*)dentry->inode_number);
        inode_num = (uint32_t)dentry->inode_number;
        printf(" Size: ");
        printf("%u", get_size(inode_num));
        printf("\n");
    }

    if(read_dentry_by_name((const uint8_t*)"cat", dentry) == 0){ // 0 means function call was a success
        printf(" File Name: ");
        printf("%s", (uint8_t*)dentry->file_name);
        printf(" File Type: ");
        printf("%u", (uint32_t*)dentry->file_type);
        printf(" Inode: ");
        printf("%u", (uint32_t*)dentry->inode_number);
        inode_num = (uint32_t)dentry->inode_number;
        printf(" Size: ");
        printf("%u", get_size(inode_num));
        printf("\n");
    }
}


void print_file_2(){
   TEST_HEADER;
   dentry_t* dentry;
    if(read_dentry_by_name((const uint8_t*)"verylargetextwithverylongname.txt", dentry) == 0){
        printf("File Name: ");
        printf("%s", (uint8_t*)dentry->file_name);
        printf("File Type: ");
        printf("%u", (uint32_t*)dentry->file_type);
        printf("Inode: ");
        printf("%u", (uint32_t*)dentry->inode_number);
    }
}
// print file
void print_file_index(){
   TEST_HEADER;
   dentry_t* dentry;
   uint32_t index = 0; // 0 because this is the starting index
   uint32_t inode_num;
    clear();
    printf("\n");

    while(1){
        if(read_dentry_by_index(index, dentry) == 0){ // 0 because this means function is successful
            printf(" File Name: ");
            printf("%s", (uint8_t*)dentry->file_name);
            printf(" File Type: ");
            printf("%u", (uint32_t*)dentry->file_type);
            printf(" Inode: ");
            printf("%u", (uint32_t*)dentry->inode_number);
            inode_num = (uint32_t)dentry->inode_number;
            printf(" Size: ");
            printf("%u", get_size(inode_num));
            printf("\n");
            index++;
        }
        else{
            printf("Invalid Index. No more dentries to print.");
            break;
        }
    }
}


void read_write_open_close(){
	TEST_HEADER;
	char* fname;
	uint32_t index;
	int32_t length;
	int32_t fd =1;
	uint8_t buf[48*1024]; // 4mb
    uint8_t bytes_read;
	fname = "ls";
	//fname = "verylargetextwithverylongname.tx";
    //fname = "frame1.txt";
	if(file_open((uint8_t*)fname)==0){ // this works 
		//printf("sucesss");
		index = file_dentry.inode_number;
		length = inode_array[index].size;
		//printf("content at index %d w length %d is:\n", index, length);
        // 
		bytes_read = file_read(fd,(uint8_t*)&buf, length);
		clear();
		printf("\n");
        //printf("length %d and bytes %d:\n", length, bytes_read);
		print_buffer(buf,length);
		//printf("read was successful");
	}
}

// read data 
void print_read_data(){
   TEST_HEADER;
   int i;
   uint32_t inode = 3;
   uint32_t offset = 0;
   uint8_t buffer[12];
   uint32_t length = 10;
    // if(read_data(inode, offset, buffer, length) > 0){
    //     printf("File Name: ");
    //     printf("%s", (uint8_t*)dentry->file_name);
    //     printf("File Type: ");
    //     printf("%u", (uint32_t*)dentry->file_type);
    //     printf("Inode: ");
    //     printf("%u", (uint32_t*)dentry->inode_number);
    // }
    int32_t result = read_data(inode, offset, buffer, length);
    for(i = 0; i < length; i++){
        //printf("%c", buffer[i]);
		//putc1(buffer[i]);
    }
    printf("%d", result);
}








/* Checkpoint 3 tests */
void test_execute(){
    TEST_HEADER;
    int32_t fd;
    uint8_t buf[BUFF_SIZE];
	int32_t nbytes = 10;
    terminal_read(fd, (uint8_t *)buf, nbytes);
    execute((uint8_t *)buf);
}
void print_pcb(pcb_t pcb){
    int i;
    for(i = 0; i < 8; i++){
        if(pcb.file_array[i].file_op_table_ptr == NULL){
            printf("Index %d is not in use", i);
        }else{
            printf(" File Pointer: %d", pcb.file_array[i].file_op_table_ptr);
            printf(" File Type: %d", pcb.file_array[i].file_inode);
            printf(" File Position: %d", pcb.file_array[i].file_position);
            printf(" File Flags: %d", pcb.file_array[i].file_flags);
            // printf("\n");
        }
        printf("\n");
    }
}

void test_open(){
    clear();
    TEST_HEADER;
    pcb_t pcb;
    initialize_pcb(0);
    open((const uint8_t*)".");
    open((const uint8_t*)"rtc");
    open((const uint8_t*)"shell");
    open((const uint8_t*)"frame0.txt");
    pcb = get_pcb(0);
    print_pcb(pcb);
}

void test_close(){
    TEST_HEADER;
    pcb_t pcb;
    int result;

    // Try to close invalid file
    result = close(0); // 0 is valid, opened index that is not allowed to be closed
    if(result == -1){
        printf("ERROR: closing index 0 or 1 is not allowed");
        printf("\n");
    }

    // Close files
    result = close(2); // 2 is a valid, opened index
    if(result == -1){ // check if close returns failure
        printf("ERROR: Failed Close at index %d", 2);
        printf("\n");
    }
    else{
        printf("Successful close at index %d", 2);
        printf("\n");
    }

    result = close(4); // 4 is a valid, opened index
    if(result == -1){ // check if close returns failure
        printf("ERROR: Failed Close at index %d", 4);
        printf("\n");
    }
    else{
        printf("Successful close at index %d", 4);
        printf("\n");
    }

    result = close(6); // 6 is a valid, BUT unopened, index
    if(result == -1){ // check if close returns failure
        printf("ERROR: Failed Close at index %d", 6);
        printf("\n");
    }
    else{
        printf("Successful close at index %d", 6);
        printf("\n");
    }

    result = close(8); // 8 is an invalid index
    if(result == -1){ // check if close returns failure
        printf("ERROR: Failed Close at index %d", 8);
        printf("\n");
    }
    else{
        printf("Successful close at index %d", 8);
        printf("\n");
    }

    pcb = get_pcb(0);
    print_pcb(pcb);
}

void test_read_dir(){
    TEST_HEADER;
    uint8_t buf[32]; // 32 is max size of name for directory
    int32_t nbytes = 32; // max of 32 bytes can be read for directory
    int32_t fd = 2; // 2 corresponds to "." directory type file
    int result;
    int i;

    result = read(fd, buf, nbytes);
    for(i = 0; i < result; i++){
        putc(buf[i]);
    }
    printf(" Bytes Read: ");
    printf("%d", result);
    printf("\n");
}

void test_read_reg(){
    TEST_HEADER;
    uint8_t buf[BUFF_SIZE];
    int32_t nbytes = BUFF_SIZE;
    int32_t fd = 5; // 5 corresponds to opened reg file
    int result;
    int i;

    result = read(fd, buf, nbytes);
    for(i = 0; i < result; i++){
        putc(buf[i]);
    }
    printf(" Bytes Read: ");
    printf("%d", result);
    printf("\n");
}

void test_write_dir(){
    TEST_HEADER;
    uint8_t buf[32]; // 32 is max size of name for directory
    int32_t nbytes = 32; // max of 32 bytes can be read for directory
    int32_t fd = 2; // 2 corresponds to "." directory type file
    int result;

    result = write(fd, buf, nbytes);
    if(result == -1){
        printf("write() returned -1 for dir file. SUCCESS.");
        printf("\n");
    }
}

void test_write_reg(){
    TEST_HEADER;
    uint8_t buf[BUFF_SIZE];
    int32_t nbytes = BUFF_SIZE;
    int32_t fd = 5; // 5 corresponds to opened reg file
    int result;

    result = write(fd, buf, nbytes);
    if(result == -1){
        printf("write() returned -1 for reg file. SUCCESS.");
        printf("\n");
    }
}

void test_read_write_rtc(){
    TEST_HEADER;
    clear();
    uint8_t buf[1] = {2};
    int32_t nbytes = 32; // not needed for rtc
    int32_t fd = 3; // 3 corresponds to opened rtc index
    int i;
    char c = '|';
    int freq; 
    for(freq = 2; freq < 1024; freq *= 2) //Start freq at 2 and double until 1024 (10)
    {
        buf[0] = freq;
        write(fd, buf, nbytes);
        for(i = 0; i < 16; i++){
            read(fd, buf, nbytes);
            putc(c);
        }
        putc('\n');
    }
}

void test_read_write_terminal(){

}


/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	//TEST_OUTPUT("check_kernel", check_kernel());
	//TEST_OUTPUT("check_video", check_video_memory());
	//null_paging();
	//non_existant_page();
	//idt_divide_error();
	 //idt_page_fault_exception();

	  // checkpoint 2
    //print_file_2();
	//read_write_open_close();
	//print_read_data();
	//print_file_name();
    //print_file_index();

    // checkpoint 3
    //test_execute();

	

}
