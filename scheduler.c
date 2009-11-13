#include "scheduler.h"

void check_schedule() {
    //    asm("move.w #0x2700,%SR");
    
#ifdef DEBUG
    rtx_dbug_outs("================================================\n\r");
    rtx_dbug_outs("Entering check_schedule\n\r");
#endif
    

    pcb* next = scheduler();

    
#ifdef DEBUG
    rtx_dbug_outs("[CHECK_SCHEDULE next = ");
    rtx_dbug_outs(pcb_get_name(next));
    rtx_dbug_outs("\n\r");
#endif
    

    if(next == current_process) {
        // allow current_process to continue to run

#ifdef DEBUG
        rtx_dbug_outs("[CHECK_SCHEDULE] current_process i_process =  ");
        rtx_dbug_out_int10(pcb_is_i_process(current_process));
        rtx_dbug_outs("\n\r");

        //rtx_dbug_outs("[CHECK_SCHEDULE] next == current_process \n\r");
        rtx_dbug_outs("[CHECK_SCHEDULE] Allowing current_process to continue running\n\r");
#endif
    }
    else {
        //preempts by switching next process in, and current_process out.
        //ie. switch next process to executing

        process_switch(next);
    }
    return;
}

pcb* scheduler() {
  
    // Find highest priority process on ready queue, but don't deque it
    // note: this uses a simple highest priority check to determine who to preempt. I think in this part we also need to do all those fancy scheduling techniques to avoid the problems with deadlock and such
    // If next == current_process, next points to a pcb that is NOT off a queue
    // If next != current_process, next points to a pcb that IS off a queue

#ifdef DEBUG
    rtx_dbug_outs("[SCHEDULER] current_process = ");
    rtx_dbug_outs(pcb_get_name(current_process));
    rtx_dbug_outs("\n\r");
#endif

    pcb* next = pcb_peek_highest(ready_queue);
    PRIORITY_LEVEL next_plvl = pcb_get_priority(next);
    if(next_plvl == RTX_ERROR || next == peek_pcb(NULL_PROCESS_PID)) {
        // ready_queue is empty, so load null_process
        
#ifdef DEBUG
        rtx_dbug_outs("[SCHEDULER] next = Null process \n\r");
#endif
        
        return get_pcb(NULL_PROCESS_PID);
    }

    // next != NULL and next != peek_pcb(NULL_PROCESS_PID)
    // next_plvl != RTX_ERROR
    // is_valid_priority(next_plvl) == TRUE

    PRIORITY_LEVEL curr_plvl = pcb_get_priority(current_process);
    if (curr_plvl == RTX_ERROR) {
        // implies current_process == NULL
        curr_plvl = 100; // set it so that next process is selected
    }

    //if((next_plvl >= curr_plvl) && (!pcb_is_i_process(current_process))) {
    // next has a higher or equal priority number (ie. lower or equal priority)

    if((next_plvl > curr_plvl) && (pcb_is_i_process(current_process) == FALSE) && (pcb_get_state(current_process) == RUNNING)) {
        // next has a higher priority number (ie. lower priority)
        // me: what if current_process has finished executing, but has not been cleaned up yet?

        next = current_process;
        //return current_process;
    }
    else {
        next = pcb_deque_highest(ready_queue);
    }

#ifdef DEBUG
    rtx_dbug_outs("[SCHEDULER] next = ");
    rtx_dbug_outs(pcb_get_name(next));
    rtx_dbug_outs("\n\r");
#endif

    return next;
}
