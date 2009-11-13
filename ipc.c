#include "ipc.h"

// note: responsibility of setting message type lies with the sender
int send_message(int process_ID, void * MessageEnvelope) {
    //    atomic(ON);
    current_primitive = SEND_MESSAGE_PRIMITIVE;
    swi_handler_param1 = (UINT32)process_ID;
    swi_handler_env = MessageEnvelope;
    // send_message_handler(int process_ID, void * MessageEnvelope);

    /*
      #ifdef DBIPC
      rtx_dbug_outs("[SEND_MESSAGE] process_ID = ");
      rtx_dbug_out_int16(process_ID);
      rtx_dbug_outs("\n\r");
      rtx_dbug_outs("[SEND_MESSAGE] MessageEnvelope = ");
      rtx_dbug_out_int16(MessageEnvelope);
      rtx_dbug_outs("\n\r");
      #endif
    */
    asm("TRAP #6");
    //    atomic(OFF);
    //return RTX_SUCCESS;

    return current_process->swi_handler_int_ret;
}

int send_message_handler(int process_ID, void* MessageEnvelope) {
    if(is_valid_pid(process_ID) == TRUE && MessageEnvelope != NULL) { 
        //set sender and destination pid in envelope

        
        /*
#ifdef DBIPC
        rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] current_process = ");
        rtx_dbug_outs(pcb_get_name(current_process));
        rtx_dbug_outs("\n\r");
#endif
        */

//	if (delaySend == FALSE && pcb_get_pid(current_process) == TIMER_I_PID){ //used by delayed_send() to signal whether or not to change sender_pid
	if (delaySend == FALSE){ //used by delayed_send() to signal whether or not to change sender_pid
        	envl_set_sender((envl*)MessageEnvelope, pcb_get_pid(current_process));
        }
	envl_set_destination((envl*)MessageEnvelope, process_ID);
        envl_set_sent_time((envl*)MessageEnvelope, currtime);

#ifdef DBIPC

        rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] envl sender = ");
        rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id((envl*)MessageEnvelope))));
        rtx_dbug_outs("\n\r");

        rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] envl destination = ");
        rtx_dbug_outs(pcb_get_name(peek_pcb(process_ID)));
        rtx_dbug_outs("\n\r");

        rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] envl type = ");
        rtx_dbug_outs(get_message_type_name(envl_get_message_type((envl*)MessageEnvelope)));
        rtx_dbug_outs("\n\r");
#endif

        // putting the MessageEnvelope onto the process's message queue
        pcb* tmp_pcb = peek_pcb(process_ID);
        envl_deque* tmp_msgq = pcb_get_message_queue(tmp_pcb);
        envl_enque((envl*)MessageEnvelope, tmp_msgq);

        // Check if destination process is blocked on message
        if(pcb_get_state(tmp_pcb) == BLOCKED_ON_MESSAGE) {

#ifdef DBIPC
            rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] dequeueing ");
            rtx_dbug_outs(pcb_get_name(tmp_pcb));
            rtx_dbug_outs(" from blocked_on_receive_queue\n\r");
#endif

            tmp_pcb = dequeue_from_blocked_on_receive_queue(process_ID);

#ifdef DBIPC
            /*
            rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] tmp_pcb = ");
            rtx_dbug_out_int16((UINT32)tmp_pcb);
            rtx_dbug_outs("\n\r");
            */
            if (tmp_pcb == NULL) {
                rtx_dbug_outs("    [SEND_MESSAGE_HANDLER][ERROR] tmp_pcb is NULL\n\r");
            }

            rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] Enqueueing ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(process_ID)));
            rtx_dbug_outs(" onto ready_queue\n\r");
#endif

            // enqueue target process onto ready queue at original priority
            enque_onto_ready_queue(tmp_pcb, pcb_get_priority(tmp_pcb));

            /*
#ifdef DBIPC
            rtx_dbug_outs("    [SEND_MESSAGE_HANDLER] Process ");
            rtx_dbug_outs(pcb_get_name(tmp_pcb));
            rtx_dbug_outs(" sent to ready_queue\n\r");
#endif
            */
        }
    }

    BOOL res = pcb_is_i_process(current_process);

    if(res == RTX_ERROR) {

#ifdef DBIPC
        rtx_dbug_outs("    [SEND_MESSAGE_HANDLER][WARNING] current_process is NULL\n\r");
#endif

    }
    return RTX_SUCCESS;
}

void* receive_message(int* sender_ID) {
    //    atomic(ON);
    current_primitive = RECEIVE_MESSAGE_PRIMITIVE;
    swi_handler_param1 = (UINT32)sender_ID;
    // receive_message_handler(int* sender_ID);
    asm("TRAP #6");
    //    atomic(OFF);
    //return NULL;

    return current_process->swi_handler_voidp_ret;
}

void* receive_message_handler(int* sender_ID) {

#ifdef DBIPC
    PROCESS_ID tmp_pid = INVALID_PID;
    if(sender_ID != NULL) {
        tmp_pid = *sender_ID;
        rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] sender = ");
        rtx_dbug_outs(pcb_get_name(peek_pcb(tmp_pid)));
        rtx_dbug_outs("\n\r");
    }
    else {
        rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] sender_ID == NULL\n\r");
        rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] Hence dequeueing first message, if any\n\r");
    }
#endif

    envl* tmp_envl = NULL;
    int res = 0;
    while(1) {

        res = is_envl_deque_empty(pcb_get_message_queue(current_process));
        if(res == FALSE) {
            tmp_envl = get_envl_by_sender_pid(current_process, (PROCESS_ID*)sender_ID);
            if(tmp_envl != NULL) {
                // implies message found from given sender_ID
                break;
            }
            else {
                // implies message queue is not empty, but did not find a message from the given sender_ID

#ifdef DBIPC
                rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] Message queue not empty, but did not find message from given sender_ID\n\r");
#endif

                // Put back envl on current_process's queue
                envl_enque(tmp_envl, pcb_get_message_queue(current_process));
            }
        }
        else if(res == RTX_ERROR) {
#ifdef DBIPC
            rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] current_process is NULL OR its message queue = NULL");
#endif
        }

        res = pcb_is_i_process(current_process);

        if(res == FALSE) {
            // if no message with given sender_ID, block calling process

#ifdef DBIPC
            rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] Blocking on receive \n\r");
#endif
            res = enque_onto_blocked_on_receive_queue(current_process, pcb_get_priority(current_process));

            if (res == RTX_ERROR) {
#ifdef DBIPC
                rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER][ERROR] Failed to put");
                rtx_dbug_outs(pcb_get_name(current_process));
                rtx_dbug_outs(" onto blocked_on_receive_queue\n\r");
#endif          
            }
            
            // preempt here since current_process is blocked on message
            check_schedule();
        }
        else if(res == TRUE) {

#ifdef DBIPC
            rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER][WARNING] i-process received no msg envl\n\r");
#endif

        }
        else {
            // res == RTX_ERROR
#ifdef DBIPC
            rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER][ERROR] current_process = NULL\n\r");
#endif            
        }
    }

#ifdef DBIPC
    rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] tmp_envl sender = ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id(tmp_envl))));
    rtx_dbug_outs("\n\r");

    rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] tmp_envl destination_pid = ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_destination_pid(tmp_envl))));
    rtx_dbug_outs("\n\r");

    rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] tmp_envl msg_type = ");
    rtx_dbug_outs(get_message_type_name(envl_get_message_type(tmp_envl)));
    rtx_dbug_outs("\n\r");

    rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] tmp_envl sent_time = ");
    rtx_dbug_out_int10(envl_get_sent_time(tmp_envl));
    rtx_dbug_outs("\n\r");

    rtx_dbug_outs("    [RECEIVE_MESSAGE_HANDLER] tmp_envl delivery_time = ");
    rtx_dbug_out_int10(envl_get_delivery_time(tmp_envl));
    rtx_dbug_outs("\n\r");
#endif

    // todo: fix this function's return from BOOL to int
    res = pcb_is_i_process(current_process);

    if(res == RTX_ERROR) {

#ifdef DBIPC
        rtx_dbug_outs("    [RECIEVE_MESSAGE_HANDLER][WARNING] current_process is NULL\n\r");
#endif

    }

    return (void*)tmp_envl;
}
