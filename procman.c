#include "procman.h"

int release_processor() {
    //atomic(ON);
    current_primitive = RELEASE_PROCESSOR_PRIMITIVE;
  
    asm("TRAP #6");
    //atomic(OFF);
    
    return current_process->swi_handler_int_ret;
}

int release_processor_handler() {
    return RTX_SUCCESS;
}


// switches current_process to state, and context_switch() s in in_next_proc
// note: should already be in supervisor mode when this procedure is called
void process_switch(pcb* in_next_proc) {

#ifdef DEBUG_PROCMAN
    rtx_dbug_outs("Entering Process Switch\n\r");
#endif

    // assume current_process has been moved onto the appropriate queue, and that its current state has been set in the pcb

    if(current_process != NULL && pcb_is_i_process(current_process) == FALSE) {
        PROCESS_STATE curr_state = pcb_get_state(current_process);

#ifdef DEBUG_PROCMAN
        rtx_dbug_outs("[PROCESS_SWITCH] i-process = FALSE\n\r");
        rtx_dbug_outs("[PROCESS_SWITCH] current_process state = ");
        rtx_dbug_outs(get_process_state_name(curr_state));
        rtx_dbug_outs("\n\r");
#endif
  
        if(curr_state == RUNNING || curr_state == READY) {
            if(is_pcb_on_ready_queue(current_process) == FALSE) {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS_SWITCH] Enqueing current_process = ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto ready_queue at plvl ");
                rtx_dbug_out_int10(pcb_get_priority(current_process));
                rtx_dbug_outs("\n\r");
#endif
                enque_onto_ready_queue(current_process, pcb_get_priority(current_process));
            }
            else {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS SWITCH] Not enqueing ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto ready_queue twice\n\r");
#endif
            }
        }
        else if(curr_state == BLOCKED_ON_MEMORY) {
            if(is_pcb_on_blocked_on_memory_queue(current_process) == FALSE) {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS_SWITCH] Enqueing current_process = ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto blocked_on_memory_queue at plvl ");
                rtx_dbug_out_int10(pcb_get_priority(current_process));
                rtx_dbug_outs("\n\r");
#endif 
                enque_onto_blocked_on_memory_queue(current_process, pcb_get_priority(current_process));
            }
            else {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS SWITCH] Not enqueing ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto blocked_on_memory_queue twice\n\r");
#endif
            }
        }
        else if(curr_state == BLOCKED_ON_MESSAGE) {
            if(is_pcb_on_blocked_on_receive_queue(current_process) == FALSE) {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS_SWITCH] Enqueing current_process = ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto blocked_on_receive_queue at plvl ");
                rtx_dbug_out_int10(pcb_get_priority(current_process));
                rtx_dbug_outs("\n\r");
#endif 
                enque_onto_blocked_on_receive_queue(current_process, pcb_get_priority(current_process));
            }
            else {
#ifdef DEBUG_PROCMAN
                rtx_dbug_outs("[PROCESS SWITCH] Not enqueing ");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto blocked_on_receive_queue twice\n\r");
#endif
            }
        }
        else {
#ifdef DEBUG_PROCMAN
            rtx_dbug_outs("[PROCESS_SWITCH][WARNING] current_process in unknown state\n\r");
#endif
        }
    }

    context_switch(in_next_proc);

}

void context_switch(pcb* in_next_proc) {

    if(pcb_is_i_process(current_process)) {
        current_process->state = I_PROC; // I_PROC is the non-running state for i-processe
    }

    // mask interrupts
    //asm("move.w #0x2700,%SR");

#ifdef DEBUG_PROCMAN
    rtx_dbug_outs("[CONTEXT_SWITCH] old current_process = ");
    rtx_dbug_outs(pcb_get_name(current_process));
    rtx_dbug_outs("\n\r");
#endif

    void * temp_stack_ptr = NULL;

    // Store Data Registers
    asm("move.l %d0, -(%a7)");
    asm("move.l %d1, -(%a7)");
    asm("move.l %d2, -(%a7)");
    asm("move.l %d3, -(%a7)");
    asm("move.l %d4, -(%a7)");
    asm("move.l %d5, -(%a7)");
    asm("move.l %d6, -(%a7)");
    asm("move.l %d7, -(%a7)");

    // Store Adress Registers 
    asm("move.l %a0, -(%a7)");
    asm("move.l %a1, -(%a7)");
    asm("move.l %a2, -(%a7)"); 
    asm("move.l %a3, -(%a7)"); 
    asm("move.l %a4, -(%a7)"); 
    asm("move.l %a5, -(%a7)"); 
    asm("move.l %a6, -(%a7)");

    // Save stack_pointer to the temp_stack_ptr itself
    asm("move.l %a7, %d0");      
    asm("move.l %d0, -4(%a6)");  
  
    if ( current_process != NULL ) {
        current_process->stack_pointer = temp_stack_ptr;
    }

    
#ifdef DEBUG_PROCMAN
    rtx_dbug_outs("[CONTEXT_SWITCH] Previous stack_pointer = ");
    rtx_dbug_out_int16((UINT32)temp_stack_ptr);
    rtx_dbug_outs("\n\r");
    rtx_dbug_outs("");
#endif
    
    // Switch in new process
    current_process = in_next_proc;
    current_process->state = RUNNING;

    // Set stack to new process' stack
    temp_stack_ptr = current_process->stack_pointer;

    
#ifdef DEBUG_PROCMAN
    rtx_dbug_outs("[CONTEXT_SWITCH] Current stack_pointer = ");
    rtx_dbug_out_int16((UINT32)temp_stack_ptr);
    rtx_dbug_outs("\n\r");
#endif
    

    // restore the stack pointer
    asm("move.l -4(%a6), %a7");

    // Restore Address registers
    asm("move.l (%a7)+, %a6");
    asm("move.l (%a7)+, %a5");
    asm("move.l (%a7)+, %a4");
    asm("move.l (%a7)+, %a3");
    asm("move.l (%a7)+, %a2");
    asm("move.l (%a7)+, %a1");
    asm("move.l (%a7)+, %a0");
  
    asm("move.l (%a7)+, %d7");
    asm("move.l (%a7)+, %d6");
    asm("move.l (%a7)+, %d5");
    asm("move.l (%a7)+, %d4");
    asm("move.l (%a7)+, %d3");
    asm("move.l (%a7)+, %d2");
    asm("move.l (%a7)+, %d1");
    asm("move.l (%a7)+, %d0");

#ifdef DEBUG_PROCMAN
    rtx_dbug_outs("[CONTEXT_SWITCH] free_mem_queue_cnt = ");
    rtx_dbug_out_int10(free_mem_queue_cnt);
    rtx_dbug_outs("\n\r");
    rtx_dbug_outs("Newly running process: \n\r     ");
    rtx_dbug_outs(pcb_get_name(current_process));
    rtx_dbug_outs("\n\r\n\r");
#endif
    /*
    if (pcb_is_i_process(current_process) == TRUE) {
        return;
    } else {
        // enable interrupts
        asm("move.w #0x2000,%SR");
    }
    */

}
