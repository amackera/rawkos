#include "timesrv.h"

int delayed_send(int process_ID, void* MessageEnvelope, int delay) {
    // Assume sender_id already set in MessageEnvelope
    //atomic(ON);
    current_primitive = DELAYED_SEND_PRIMITIVE;
    swi_handler_param1 = (UINT32)process_ID;
    swi_handler_param2 = (UINT32)MessageEnvelope;
    swi_handler_param3 = (UINT32)delay;
    //delayed_send_handler(int process_ID, void* MessageEnvelope, int delay);
    asm("TRAP #6");
    //atomic(OFF);
    //return RTX_SUCCESS;

    return current_process->swi_handler_int_ret;
}

int delayed_send_handler(int process_ID, void* MessageEnvelope, int delay) {
    envl_set_sent_time((envl*)MessageEnvelope, currtime);
    envl_set_delivery_time((envl*)MessageEnvelope, currtime +delay);
    envl_set_destination((envl*)MessageEnvelope, process_ID);

    enque_onto_delay_send_queue_by_delivery_time((envl*)MessageEnvelope);

    //check_schedule();
    return RTX_SUCCESS;
}
