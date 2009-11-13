#ifndef PCB_H
#define PCB_H

#include "const.h"
#include "envl_deque.h"
#include "outs.h"

typedef struct pcb {
    struct pcb* next; // Points to next PCB
    struct pcb* prev; // Points to previous PCB; using as deque to aid debugging
    PROCESS_ID pid;   // Value of Process ID
    char* name;
    PRIORITY_LEVEL priority;
    PROCESS_STATE state;
    void* stack_pointer;// Points to process's stack
    void* stack_start_addr; // todo: change all these to UINT<size_of_addr>
    void* stack_end_addr;
    void* code_pointer; // Points to location of process's code
    SINT32 reg_data[8]; // Stores value of the \# = [0,7] 32-bit data registers
    SINT32 reg_addr[8]; // Stores value of the \# = [0,7] 32-bit address registers
    SINT32 reg_pc; // Stores value of the 32-bit Program Counter (PC)
    SINT16 reg_cc; // Stores value of the 16-bit Condition Code (CC)
    SINT32 exception_stack_frame; // MSB: system byte. LSB: CCR
    BOOL i_process;
    envl_deque messages;
    int swi_handler_int_ret;
    void* swi_handler_voidp_ret;
    UINT32 stack_size;
} pcb;

BOOL pcb_is_i_process(pcb* in_pcb);

int pcb_set_state(pcb* in_pcb, PROCESS_STATE in_state);
int pcb_set_priority(pcb* in_pcb, PRIORITY_LEVEL in_plvl);
int pcb_set_stack_pointer(pcb* in_pcb, void* in_sp);
int pcb_set_stack_start_addr(pcb* in_pcb, void* in_sp);
int pcb_set_stack_end_addr(pcb* in_pcb, void* in_sp);
int pcb_set_code_pointer(pcb* in_pcb, void* in_sp);

PROCESS_ID pcb_get_pid(pcb* in_pcb);
char* pcb_get_name(pcb* in_pcb);
PROCESS_STATE pcb_get_state(pcb* in_pcb);
PRIORITY_LEVEL pcb_get_priority(pcb* in_pcb);
envl_deque* pcb_get_message_queue(pcb* in_pcb);
void* pcb_get_code_pointer(pcb* in_pcb);
void* pcb_get_stack_pointer(pcb* in_pcb);
void* pcb_get_stack_start_addr(pcb* in_pcb);
void* pcb_get_stack_end_addr(pcb* in_pcb);

//int enque_envl(pcb* in_pcb, envl* in_msgenv);
//envl* dequeue_envl(pcb* in_pcb);
int pcb_print(pcb* in_pcb);


#endif /* PCB_H */
