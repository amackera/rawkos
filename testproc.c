#include "testproc.h"

/*
#define A1_PID 7
#define B1_PID 8
#define C1_PID 9
#define A2_PID 10
#define B2_PID 11
#define C2_PID 12
#define A3_PID 13
 */

void process_a1() {
#ifdef DEBUG
    rtx_dbug_outs("================================================\n\r");
    rtx_dbug_outs("Entering process_a1\n\r");
#endif
    register_command(*"Z");

#ifdef DEBUG
    int i = 0;
    //int j = 0;
#endif

    envl* tmp_envl = NULL;
    while(1) {

#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A1] Checking for messages ( ");
      rtx_dbug_out_int16(i++);
      rtx_dbug_outs(" times run )\n\r");
#endif
      
      tmp_envl = (envl*)receive_message(NULL);

#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A1] Message received from ");
      rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id(tmp_envl))));
      rtx_dbug_outs("\n\r");
#endif

        // check if message contains "%Z" 
        char* tmp_msgtext = envl_get_msgtext(tmp_envl);

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_A1] tmp_envl msgtext = ");
        rtx_dbug_outs(tmp_msgtext);
        rtx_dbug_outs("\n\r");
#endif

        if(tmp_msgtext[0] == 0x25 /* % */ &&
           tmp_msgtext[1] == 0x5a /* Z */) {
            release_memory_block(tmp_envl);
            break;
        }
        else {
            release_memory_block(tmp_envl);
        }
    }
    int num = 0;
    char buff[33]; // max num of digits in unsigned int +1
    while(1) {
        /*
#ifdef DEBUG   
      j++;
      rtx_dbug_outs("process_a1 here2 = ");
      rtx_dbug_out_int16(j);
      rtx_dbug_outs("\n\r");
#endif
        */
        tmp_envl = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_A1] Received requested memory block\n\r");
#endif  
        envl_set_message_type(tmp_envl, COUNT_REPORT);
        itoa(num, buff, 10); // buff should be null terminated
        envl_set_msgtext(tmp_envl, buff);

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_A1] tmp_envl msgtext = ");
        rtx_dbug_outs(envl_get_msgtext(tmp_envl));
        rtx_dbug_outs("\n\r");
#endif

        envl_set_message_type(tmp_envl, MISC_MSG);
        send_message(B1_PID, (void*)tmp_envl);
        num = num +1;
        release_processor();
    }
}

void process_a2() {

    register_command(*"Y");

#ifdef DEBUG
    int i = 0;
    //int j = 0;
#endif

    envl* tmp_envl = NULL;
    while(1) {
#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A2] Checking for messages ( ");
      rtx_dbug_out_int16(i++);
      rtx_dbug_outs(" times run )\n\r");
#endif

       tmp_envl = (envl*)receive_message(NULL);

#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A2] Message received from ");
      rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id(tmp_envl))));
      rtx_dbug_outs("\n\r");
#endif

        // check if message contains "%Y" 
        char* tmp_msgtext = envl_get_msgtext(tmp_envl);
        if(tmp_msgtext[0] == 0x25 /* % */ &&
           tmp_msgtext[1] == 0x59 /* Y */) {
            release_memory_block(tmp_envl);
            break;
        }
        else {
            release_memory_block(tmp_envl);
        }
    }
    int num = 0;
    char buff[33]; // max num of digits in unsigned int +1
    while(1) {
        tmp_envl = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_A2] Received requested memory block");
#endif  
        envl_set_message_type(tmp_envl, COUNT_REPORT);
        itoa(num, buff, 10); // buff should be null terminated
        envl_set_msgtext(tmp_envl, buff);
        envl_set_message_type(tmp_envl, MISC_MSG);
        send_message(B2_PID, (void*)tmp_envl);
        num = num +1;
        release_processor();
    }
}

void process_a3() {

    register_command(*"X");



#ifdef DEBUG
    int i = 0;
    //int j = 0;
#endif

    envl* tmp_envl = NULL;
    while(1) {

#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A3] Checking for messages ( ");
      rtx_dbug_out_int16(i++);
      rtx_dbug_outs(" times run )\n\r");
#endif

        tmp_envl = (envl*)receive_message(NULL);


#ifdef DEBUG
      rtx_dbug_outs("[PROCESS_A3] Message received from ");
      rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id(tmp_envl))));
      rtx_dbug_outs("\n\r");
#endif

        // check if message contains "%X" 
        char* tmp_msgtext = envl_get_msgtext(tmp_envl);
        if(tmp_msgtext[0] == 0x25 /* % */ &&
           tmp_msgtext[1] == 0x58 /* X */) {
            release_memory_block(tmp_envl);
            break;
        }
        else {
            release_memory_block(tmp_envl);
        }
    }
    int num = 0;
    char buff[33]; // max num of digits in unsigned int +1
    while(1) {
        tmp_envl = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_A3] Received requested memory block");
#endif  
        envl_set_message_type(tmp_envl, COUNT_REPORT);
        itoa(num, buff, 10); // buff should be null terminated
        envl_set_msgtext(tmp_envl, buff);
        envl_set_message_type(tmp_envl, MISC_MSG);
        send_message(B2_PID, (void*)tmp_envl);
        num = num +1;
        release_processor();
    }
}

void process_b1() {
    while(1) {

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_B1] Checking for messages\n\r");
#endif

        envl* tmp_envl = receive_message(NULL);

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_B1] Got a message envelope. Now sending it to C1\n\r");
#endif

        // envl should already have a message type set
        send_message(C1_PID, (void*)tmp_envl);
    }
}

void process_b2() {
    while(1) {

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_B2] Checking for messages\n\r");
#endif

        envl* tmp_envl = receive_message(NULL);

#ifdef DEBUG
        rtx_dbug_outs("[PROCESS_B2] Got a message envelope. Now sending it to C2\n\r");
#endif

        // envl should already have a message type set
        send_message(C2_PID, (void*)tmp_envl);
    }
}

void process_c1() {
    envl_deque local_msg_q;
    local_msg_q.head = NULL;
    local_msg_q.tail = NULL;
    process_c1_local_msg_q_addr = &local_msg_q;

    envl* tmp_envl = NULL;
    while(1) {
        //int is_envl_deque_empty(envl_deque* in_deque)
        if(is_envl_deque_empty(&local_msg_q)) {
            tmp_envl = (envl*)receive_message(NULL);
        }
        else {
            tmp_envl = envl_dequeue(&local_msg_q);
        }

        if(envl_get_message_type(tmp_envl) == COUNT_REPORT) {
            int tmp_num = atoi(envl_get_msgtext(tmp_envl)); // todo: ensure msgtext is null terminated

#ifdef DEBUG
            rtx_dbug_outs("[PROCESS_C1] tmp_num = ");
            rtx_dbug_out_int10(tmp_num);
            rtx_dbug_outs("\n\r");
#endif
            if((tmp_num % 20) == 0) {
                envl* tmp_envl2 = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_C1] Received requested memory block\n\r");
#endif  
                envl_set_msgtext(tmp_envl2, "Process C");
                envl_set_message_type(tmp_envl2, CRT_DISPLAY);
                send_message(CRT_DISPLAY_PID, tmp_envl2);

                // sleep for 10 seconds
                tmp_envl = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_C1] Received requested memory block\n\r");
#endif  
                envl_set_message_type(tmp_envl, WAKEUP10);
                delayed_send(C1_PID, (void*)tmp_envl, 10000);
                while(1) {
                    // block and let other processes execute
                    tmp_envl = (envl*)receive_message(NULL);
                    if(envl_get_message_type(tmp_envl) == WAKEUP10) {
                        break;
                    }
                    else {
                        envl_enque(tmp_envl, &local_msg_q);
                    }
                }
            }
        }
        release_memory_block(tmp_envl);
        release_processor();
    }
}

void process_c2() {
    envl* tmp_envl2 = (envl*)request_memory_block();
#ifdef DEBUG   
      rtx_dbug_outs("[PROCESS_C2] Received requested memory block\n\r");
#endif  
    envl_deque local_msg_q;
    local_msg_q.head = NULL;
    local_msg_q.tail = NULL;
    process_c2_local_msg_q_addr = &local_msg_q;

    envl* tmp_envl = NULL;
    while(1) {
        //int is_envl_deque_empty(envl_deque* in_deque)
        if(is_envl_deque_empty(&local_msg_q)) {
            tmp_envl = (envl*)receive_message(NULL);
        }
        else {
            tmp_envl = envl_dequeue(&local_msg_q);
        }

        BOOL free = TRUE;
        if(envl_get_message_type(tmp_envl) == COUNT_REPORT) {
            int tmp_num = atoi(envl_get_msgtext(tmp_envl)); // todo: ensure msgtext is null terminated

#ifdef DEBUG
            rtx_dbug_outs("[PROCESS_C2] tmp_num = ");
            rtx_dbug_out_int10(tmp_num);
            rtx_dbug_outs("\n\r");
#endif

            if((tmp_num % 20) == 0) {
                envl_set_msgtext(tmp_envl, "Process C");
                envl_set_message_type(tmp_envl, CRT_DISPLAY);
                send_message(CRT_DISPLAY_PID, tmp_envl);

                free = FALSE;

                // sleep for 10 seconds
                tmp_envl2 = (envl*)request_memory_block();
                envl_set_message_type(tmp_envl2, WAKEUP10);
                delayed_send(C1_PID, (void*)tmp_envl2, 10000);
                while(1) {
                    // block and let other processes execute
                    tmp_envl2 = (envl*)receive_message(NULL);
                    if(envl_get_message_type(tmp_envl2) == WAKEUP10) {
                        break;
                    }
                    else {
                        envl_enque(tmp_envl2, &local_msg_q);
                    }
                }
            }
        }
        if(free == TRUE) {
            release_memory_block(tmp_envl);
        }
        release_processor();
    }
}
