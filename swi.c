#include "swi.h"

void swi_handler() {


    // mask interrupts (enable atomic) NOT NECESSARY (Done in trap_entry.s)
    asm("move.w #0x2700,%SR");    


#ifdef DEBUG
    /*
      rtx_dbug_outs("swi_handler_param1 =  ");
      rtx_dbug_out_int16((UINT32)swi_handler_param1);
      rtx_dbug_outs("\n\r");

      rtx_dbug_outs("swi_handler_param2 =  ");
      rtx_dbug_out_int16((UINT32)swi_handler_param2);
      rtx_dbug_outs("\n\r");

      rtx_dbug_outs("swi_handler_param3 =  ");
      rtx_dbug_out_int16((UINT32)swi_handler_param3);
      rtx_dbug_outs("\n\r");

      rtx_dbug_outs("swi_handler_env =  ");
      rtx_dbug_out_int16((UINT32)swi_handler_env);
      rtx_dbug_outs("\n\r");
    */

    /*
      if(swi_handler_param1 != NULL){
      rtx_dbug_outs("swi_handler_param1 =  ");
      rtx_dbug_out_int10(swi_handler_param1);
      rtx_dbug_outs("\n\r");
      }
      else {
      rtx_dbug_outs("swi_handler_param1 = NULL\n\r");
      }

      if(swi_handler_param2 != NULL){
      rtx_dbug_outs("swi_handler_param2 =  ");
      rtx_dbug_out_int10(swi_handler_param2);
      rtx_dbug_outs("\n\r");
      }
      else {
      rtx_dbug_outs("swi_handler_param2 = NULL\n\r");
      }

      if(swi_handler_param3 != NULL){
      rtx_dbug_outs("swi_handler_param3 =  ");
      rtx_dbug_out_int10(swi_handler_param3);
      rtx_dbug_outs("\n\r");
      }
      else {
      rtx_dbug_outs("swi_handler_param3 = NULL\n\r");
      }

      if(swi_handler_env != NULL){
      rtx_dbug_outs("swi_handler_env =  ");
      rtx_dbug_out_int16((UINT32)swi_handler_env);
      rtx_dbug_outs("\n\r");
      }
      else {
      rtx_dbug_outs("swi_handler_env = NULL\n\r");
      }
    */
#endif

    /*
    // "Passing" parameters from a primitive to swi_handler()
    // This is probably unsafe, but as long as it works, it's safe
    int current_primitive;
    UINT32 swi_handler_param1;
    UINT32 swi_handler_param2;
    UINT32 swi_handler_param3;
    int swi_handler_int_ret;
    void* swi_handler_voidp_ret;
    */
    switch(current_primitive) {
    case SEND_MESSAGE_PRIMITIVE:
        // int send_message_handler(int process_ID, void* MessageEnvelope);
        current_process->swi_handler_int_ret = send_message_handler((int)swi_handler_param1, swi_handler_env);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
    	break;
    
    case RECEIVE_MESSAGE_PRIMITIVE:
        // void* receive_message_handler(int* sender_ID);
        current_process->swi_handler_voidp_ret = receive_message_handler((int*)swi_handler_param1);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
        break;
    
    case REQUEST_MEMORY_BLOCK_PRIMITIVE:
        // void* request_memory_block_handler();
        current_process->swi_handler_voidp_ret = request_memory_block_handler();
        break;
    
    case RELEASE_MEMORY_BLOCK_PRIMITIVE:
        // int release_memory_block_handler(void* MemoryBlock);
        current_process->swi_handler_int_ret = release_memory_block_handler((void*)swi_handler_param1);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
        break;
    
    case RELEASE_PROCESSOR_PRIMITIVE:
        // int release_processor();
        current_process->swi_handler_int_ret = release_processor_handler();
        break;
    
    case DELAYED_SEND_PRIMITIVE:
        // int delayed_send_handler(int process_ID, void* MessageEnvelope, int delay);
        current_process->swi_handler_int_ret = delayed_send_handler((int)swi_handler_param1, (void*)swi_handler_param2, (int)swi_handler_param3);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
        swi_handler_param2 = 0;
        swi_handler_param3 = 0;
        break;
    
    case SET_PROCESS_PRIORITY_PRIMITIVE:

        // int set_process_priority_handler(int process_ID, int priority);
        current_process->swi_handler_int_ret = set_process_priority_handler((int)swi_handler_param1, (int)swi_handler_param2);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
        swi_handler_param2 = 0;
        break;
    
    case GET_PROCESS_PRIORITY_PRIMITIVE:
        // int get_process_priority_handler(int process_ID);
        current_process->swi_handler_int_ret = get_process_priority_handler((int)swi_handler_param1);
        // Should be done with param so clearing it. This should still be atomic so should be fine.
        swi_handler_param1 = 0;
        break;

    case RTX_NORMAL:

#ifdef DEBUG
        rtx_dbug_outs("[RTX_NORMAL]\n\r");
#endif

        current_process = NULL;
        //        asm("move.w #0x2000, %SR");
        check_schedule();
        break;

    case RTX_INIT:
        check_schedule();
        //asm("move.w #0x2000, %SR");
        break;

    case CONTEXT_SWITCH_UART:

#ifdef DEBUG
        //rtx_dbug_outs("Supposed to context switch to UART\n\r");
#endif
        // current_process should not be an i-process
        process_switch(get_pcb(UART_I_PID));
        //        uart_i_process();
        //check_schedule();
        break;

    case CONTEXT_SWITCH_TIMER:
#ifdef DEBUG
        //rtx_dbug_outs("Supposed to context switch to timer\n\r");
#endif
        // current_process should not be an i-process
        process_switch(get_pcb(TIMER_I_PID));
        //timer_i_process();
        //check_schedule();
        break;

    default:
        // todo: give functionality here
        break;
    
    }



    if(pcb_is_i_process(current_process) == TRUE) {
        return;
    } else {
        check_schedule();
        return;
    }





    /*
    if((pcb_is_i_process(current_process) == FALSE        
        || pcb_is_i_process(current_process) == RTX_ERROR)
#ifdef KINSON
       && current_primitive != GET_PROCESS_PRIORITY_PRIMITIVE
#endif
       ) {
        // If current_process == NULL or NOT I-PROCESS
        
#ifdef DEBUG
        rtx_dbug_outs("[SWI_HANDLER] Calling check_schedule()\n\r");
#endif
        
        // unmask interrupts (disable atomic)
        //asm("move.w #0x2000,%SR");
        
        check_schedule();
    } else if (pcb_is_i_process(current_process) == TRUE) {
            
            // ELSE we are in an I-process
    
#ifdef DEBUG
            rtx_dbug_outs("[SWI_HANDLER] Leaving swi_handler to I-Process\n\r");
#endif
            // unmask interrupts (disable atomic)
            //asm("move.w #0x2000,%SR");
            return;
    }
    */

    return;
}
