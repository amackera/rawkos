#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "const.h"
#include "pcb_pdeque.h"
#include "envl_deque.h"

pcb* current_process; // Refers to currently executing process (ie. to PCB)
int pcbcnt;           // Number of PCBs in the system
int envlcnt;          // Number of envelopes in the system
BOOL clock_state;     // true = ON, false = OFF
SYSTIME currtime; // Updated in the Timer I-process // has units of seconds
BOOL delaySend; //Used for delayed_send() to signal whether or not the sender of the message should be changed
SYSTIME delaytime;

pcb* pcb_table[DEFAULT_PCBCNT]; // table of pointers to PCBs
envl* envl_table[DEFAULT_ENVLCNT]; // table of pointers to envelopes

// priority levels implicitly 1-to-1 with array indicies
// implicitly 0 is highest priority
pcb_deque blocked_on_receive_queue[NUM_PRIORITY]; // processes waiting for a message
pcb_deque blocked_on_memory_queue[NUM_PRIORITY]; // processes waiting for memory to send messages
pcb_deque ready_queue[NUM_PRIORITY];

envl_deque* free_mem_queue;
int free_mem_queue_cnt; // number of elements on free_mem_queue
envl_deque* blocked_mem_queue;
int blocked_mem_queue_cnt;
envl_deque* delay_send_queue; // should be always sorted by least delay
int delay_send_queue_cnt;

// PCB Stuff
// __end is where the compiler thinks the free memory starts
extern void * __end;
// goAddress is where we think the OS will start to reside in memory
//void * goAddress = 10100000;  
// startAddress is the start of free memory
const void * pcbStartAddress;
void * pcbEndAddress;
void * envlEndAddress;

// For communication from a user visible primative to swi_handler()
// Since atomic mode is turned on, then trap is called, then swi_handler() calls the appropriate kernel primtive, these values should not change in-between
int current_primitive;
UINT32 swi_handler_param1;
UINT32 swi_handler_param2;
UINT32 swi_handler_param3;
//env swi_handler_env;
void* swi_handler_env;

// Likewise, when swi_handler() returns, it is still in atomic mode, so these values should not change in-between
// For the return value, this is only set after a kernel primitive finishes, at which point swi_handler() returns. Only one of these values should be returned.
int swi_handler_int_ret;
void* swi_handler_voidp_ret;
// me: I do this to avoid using the current_process's stack, and to avoid having to write assembly

//variable to count the number of times the timer interrupt fires for the timer_i_process
UINT32 timer_count;

//variable used for the acknowledgment of serial interrupt 
BYTE uart_ack;
char tmp_char;
BOOLEAN key_in;

int cmd_table_cnt;

envl_deque* process_c1_local_msg_q_addr;
envl_deque* process_c2_local_msg_q_addr;

#endif /* GLOBALVAR_H */
