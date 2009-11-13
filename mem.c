#include "mem.h"

void* request_memory_block() {
    //atomic(ON);
    current_primitive = REQUEST_MEMORY_BLOCK_PRIMITIVE;
    // request_memory_block_handler();
#ifndef ASM_OFF
    asm("TRAP #6");
#endif
    //atomic(OFF);
    // return NULL;

    return current_process->swi_handler_voidp_ret;
}

void* request_memory_block_handler() {

  envl* tmpenvl;
    while(1) {

#ifdef DEBUG
        rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK] Entering...\n\r");
#endif

        int res = pcb_is_i_process(current_process);
	if(free_mem_queue_cnt <= 5) {
	  if(res == TRUE) {
	    tmpenvl = dequeue_from_free_mem_queue();
	  }
	  else {
	    check_schedule();
	    return NULL;
	  }
	}
	else {
	  tmpenvl = dequeue_from_free_mem_queue();
	}

        if(res == TRUE) {
            if(tmpenvl == NULL) {
#ifdef DEBUG
                rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK][ERROR] No mem to allocate to an i-process\n\r");
#endif
            }
            else {
#ifdef DEBUG
                rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK] Returning envelope to I-Process\n\r");
#endif
                
                return (void*)tmpenvl;
            }
        }
        else if(res == FALSE) {
            if(tmpenvl == NULL) {
                enque_onto_blocked_on_memory_queue(current_process, pcb_get_priority(current_process));
                /*
                check_schedule(); // preempt here
                */
            }
            else {
                /*
#ifdef DEBUG
                rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK] Returning envelope to process later\n\r");
#endif
                check_schedule();
#ifdef DEBUG
                rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK] Returning envelope to process now\n\r");
#endif
                */
                return (void*)tmpenvl;
            }
        }
        else { // res == RTX_ERROR
#ifdef DEBUG
            rtx_dbug_outs("    [REQUEST_MEMORY_BLOCK][ERROR] current_process = NULL\n\r");
#endif
        }
    }
}

int release_memory_block(void* MemoryBlock) {
    //atomic(ON);
    current_primitive = RELEASE_MEMORY_BLOCK_PRIMITIVE;
    swi_handler_param1 = (UINT32)MemoryBlock;
    // release_memory_block_handler(void* MemoryBlock);
#ifndef ASM_OFF
    asm("TRAP #6");
#endif
    //atomic(OFF);
    // return RTX_SUCCESS;


    return current_process->swi_handler_int_ret;
}

int release_memory_block_handler(void* MemoryBlock) {
    if(MemoryBlock == NULL) {
        return RTX_ERROR;
    }

    envl_clear((envl*)MemoryBlock);
    enque_onto_free_mem_queue((envl*)MemoryBlock);

    // check if any processes blocked on memory, and set to ready if there are
    if(!is_pcb_pdeque_empty(blocked_on_memory_queue)) {
        pcb* tmp_pcb = pcb_deque_highest(blocked_on_memory_queue);
        if(tmp_pcb == NULL) {
            // all is well; check schedule next
        }
        else {
            // enque tmp_pcb onto ready queue at the priority it originally had
            enque_onto_ready_queue(tmp_pcb, pcb_get_priority(tmp_pcb));
        }        
    }
    //check_schedule(); // will determine which process should execute next. tmp_pcb may not necessarily have highest priority on the ready_queue, even if it had the highest priority on the blocked_on_memory_queue. Such an issue is up to the scheduler to decide
    return RTX_SUCCESS;
}
