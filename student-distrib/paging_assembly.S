# this is the assemnly file for paging
# need this so we can manipulate cr0,cr3 and cr4 

 #define ASM     1

# define globals
#  .globl page_enabler # enables page
 .globl load_directory
 
# got this code from OS_DEV (https://wiki.osdev.org/Paging#Page_Directory)
# this code loads the directory and enables paging, and enables the pse for 4mib pages
# cr3 holds the mem address, cr4 takes charge of the pse, cr0 takes charge of the enabling
 load_directory:
    pushl %ebp
    movl %esp,%ebp 
    # pushl %eax 
    # we need to put address of page directory into eax 
    movl 8(%esp), %eax # we want mem address
    movl %eax , %cr3 

    # enable pse
   # movl %eax, %cr4
   # or eax, 0x00000010
   movl %cr4, %eax
    orl $0x00000010, %eax # got this number from os dev ,, this enables PSE (4 MiB pages)
    movl %eax , %cr4 
    # movl %cr4, %eax

    movl %cr0, %eax 
    orl $0x80000001, %eax # got this number from os dev ,, enables paging 
    movl %eax, %cr0

   # leave
    # popl %eax 
    movl %ebp, %esp
    popl %ebp
    ret 



