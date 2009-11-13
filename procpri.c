#include "procpri.h"

// Set Process Priority Parameters:
// * int process_ID: PID of process requiring a priority level change.
// * int priority: The value of the new priority level
// Output: RTX_SUCCESS if completed successfully
// Description: Change priority level of the process given by process_ID
int set_process_priority(int process_ID, int priority) {
    //    atomic(ON);
    current_primitive = SET_PROCESS_PRIORITY_PRIMITIVE;
    swi_handler_param1 = process_ID;
    swi_handler_param2 = priority;

#ifndef ASM_OFF
    asm("TRAP #6");
#endif /*ASM_OFF*/
 
    //atomic(OFF);
    // return RTX_SUCCESS;

    return current_process->swi_handler_int_ret;
}

int set_process_priority_handler(int process_ID, int priority) {
    // note: to be honest if we have an error then what? lets just assume < 0 = min; > max = max    
#ifdef DBPROCPRI
    rtx_dbug_outs("***[SET PRIORITY HANDLER] Old priority was ");
    rtx_dbug_out_int10((UINT32)pcb_get_priority(peek_pcb(process_ID)));
    rtx_dbug_outs("\n\r");
#endif
    if(!is_valid_priority(priority) ||
       pcb_get_priority(peek_pcb(process_ID)) == PRIORITY_LEVEL_4) {
#ifdef DBPROCPRI
		rtx_dbug_outs("[SET PRIORITY HANDLER] Priority NOT valid \n\r");
#endif
        return RTX_ERROR; // ie. invalid priority
    }

    pcb* tmp_pcb = peek_pcb(process_ID);
    if(tmp_pcb == NULL) {
#ifdef DBPROCPRI
        rtx_dbug_outs("[SET PRIORITY HANDLER] Did not find pcb with pid ");
        rtx_dbug_out_int10(process_ID);
        rtx_dbug_outs("\n\r");
#endif
        return RTX_ERROR;
    }

    int res = change_priority(tmp_pcb, priority); // changes priority in tmp_pcb and moves it to the priority level specified by priority
    if(res == RTX_ERROR) {
#ifdef DBPROCPRI
        rtx_dbug_outs("[SET PRIORITY HANDLER] Attempted to change to invalid priority\n\r");
#endif 
        return RTX_ERROR;
    }
    
#ifdef DBPROCPRI
    rtx_dbug_outs("***[SET PRIORITY HANDLER] New priority of ");
    rtx_dbug_outs(pcb_get_name(tmp_pcb));
    rtx_dbug_outs(" set to ");
    rtx_dbug_out_int10(tmp_pcb -> priority);
    rtx_dbug_outs("\n\r");
#endif

    //check_schedule(); // premption done here
    return RTX_SUCCESS;
}


// Get Process Priority Parameters:
// * int process_ID: Retrieve priority of given process given by PID
// Output: Priority value of given process_ID; RTX_ERROR otherwise
// Description: Retrieve the priority of the process specified by process_ID
int get_process_priority(int process_ID) {
#ifdef DBPROCPRI
    rtx_dbug_outs("[GET PRIORITY] Entering... \n\r");
#endif
    //    atomic(ON);
    current_primitive = GET_PROCESS_PRIORITY_PRIMITIVE;
    swi_handler_param1 = process_ID;
    // get_process_priority_handler(int process_ID);
#ifndef ASM_OFF
    asm("TRAP #6");
#endif /* ASM_OFF */
    //    atomic(OFF);
    //return RTX_SUCCESS;
#ifdef DBPROCPRI
    rtx_dbug_outs("[GET PRIORITY] Leaving... \n\r");
#endif	

    return current_process->swi_handler_int_ret;
}

int get_process_priority_handler(int process_ID) {
#ifdef DBPROCPRI
    rtx_dbug_outs("***[GET PRIORITY HANDLER] Entering... \n\r");
    rtx_dbug_outs("***[GET PRIORITY HANDLER] Retrieving priority for process: ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(process_ID)));
    rtx_dbug_outs("\n\r");
#endif

    //TODO(kinson): Seems like if it returns RTX_ERROR then GG hangs
    if(!is_valid_priority(pcb_get_priority(peek_pcb(process_ID)))) {
#ifdef DBPROCPRI
        rtx_dbug_outs("[GET PRIORITY HANDLER] Priority is NOT valid, priority is ");
        rtx_dbug_out_int10(pcb_get_priority(peek_pcb(process_ID)));
        rtx_dbug_outs("\n\r");
#endif
        return RTX_ERROR;
    }
    pcb* tmp_pcb = peek_pcb(process_ID);
    if(tmp_pcb == NULL) {

#ifdef DBPROCPRI
        rtx_dbug_outs("[GET PRIORITY HANDLER] Did not find pcb with pid ");
        rtx_dbug_out_int10(process_ID);
        rtx_dbug_outs("\n\r");
#endif

        return RTX_ERROR;
    }
    else {
        return (int)pcb_get_priority(tmp_pcb);
    }
}
