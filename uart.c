#include "uart.h"

void c_uart_handler() {
#ifdef DEBUG_IPROC
    rtx_dbug_outs("================================================\n\r");
    rtx_dbug_outs("[UART INTERRUPT HANDLER] Entered...\n\r");
#endif
    uart_ack = SERIAL1_UCSR; //acknowledge the interrupt

    if(uart_ack & 1) {
        key_in = FALSE;
        tmp_char = SERIAL1_RD;
    } else if(uart_ack & 4) {
        key_in = TRUE;
        SERIAL1_IMR = 0x02;
    }

#ifdef DEBUG_IPROC
    rtx_dbug_outs("[UART INTERRUPT HANDLER] key_in = '");
    rtx_dbug_out_int10(key_in);
    rtx_dbug_outs("'\n\r");
    rtx_dbug_outs("[UART INTERRUPT HANDLER]...Leaving\n\r");
#endif

    current_primitive = CONTEXT_SWITCH_UART;
    asm("TRAP #6");
//	process_switch(uart_i_process);

}

void uart_i_process() {
    while(1) {
        // mask interrupts (enable atomic)
        asm("move.w #0x2700,%SR");
#ifdef DEBUG_IPROC
        rtx_dbug_outs("================================================\n\r");
        rtx_dbug_outs("[UART] Entered...\n\r");
#endif
    
        char tmp_char_in[2] = {0,0};
  
#ifdef DEBUG_IPROC
        rtx_dbug_outs("[UART] SERIAL1_UCSR = '");
        rtx_dbug_out_int16(uart_ack);
        rtx_dbug_outs("'\n\r");
#endif
  
  
        if(!key_in) { // check to see if a new character is received
	    if (tmp_char == 0x0d){
	   	tmp_char_in[0] = 0x0a;
	    } else {
		tmp_char_in[0] = tmp_char;
	    }
            key_in = TRUE;
    
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] SERIAL1_RD = '");
            rtx_dbug_outs(tmp_char_in);
            rtx_dbug_outs("'\n\r");
#endif

#ifdef DEBUG_HOTKEYS
            if(tmp_char_in[0] == 0x21) { // !
                rtx_dbug_outs("[UART] Printing ready_queue\n\r");
                print_ready_queue();
            }
            if(tmp_char_in[0] == 0x40) { // @
                rtx_dbug_outs("[UART] Printing blocked_on_receive_queue\n\r");
                print_blocked_on_receive_queue();
            }
            if(tmp_char_in[0] == 0x23) { // #
                rtx_dbug_outs("[UART] Printing blocked_on_memory_queue\n\r");
                print_blocked_on_memory_queue();
            }
            if(tmp_char_in[0] == 0x24) { // $
                // todo: print out message log
            }
            // skipping '%' since this is used for commands
            if(tmp_char_in[0] == 0x5e) { // ^
                // Prints out all processes, their priority, and their state
                rtx_dbug_outs("[UART] Printing out all ");
                rtx_dbug_out_int10(pcbcnt);
                rtx_dbug_outs(" pcbs in the system\n\r");
                print_pcb_table();
            }
            if(tmp_char_in[0] == 0x26) { // &
                rtx_dbug_outs("[UART] Printing all pcb queues\n\r");
                rtx_dbug_outs("[UART] Printing ready_queue\n\r");
                print_ready_queue();
                rtx_dbug_outs("[UART] Printing blocked_on_memory_queue\n\r");
                print_blocked_on_receive_queue();
                rtx_dbug_outs("[UART] Printing blocked_on_memory_queue\n\r");
                print_blocked_on_memory_queue();
            }
            if(tmp_char_in[0] == 0x2a) { // *
                rtx_dbug_outs("[UART] Printing out all ");
                rtx_dbug_out_int10(envlcnt);
                rtx_dbug_outs(" envl in the system\n\r");
                print_envl_table();
            }
            if(tmp_char_in[0] == 0x28) { // (
                rtx_dbug_outs("[UART] Printing out all messages on queues\n\r");
                rtx_dbug_outs("[UART] Printing out all ");
                rtx_dbug_out_int10(delay_send_queue_cnt);
                rtx_dbug_outs(" envls on delay_send_queue\n\r");
                envl_print_deque(delay_send_queue);

                rtx_dbug_outs("[UART] Printing out messages (if any) from a process's message queue\n\r");
                int i = 0;
                for(; i<pcbcnt; i++) {
                    if(is_envl_deque_empty(pcb_get_message_queue(pcb_table[i])) == FALSE) {
                        rtx_dbug_outs(pcb_get_name(pcb_table[i]));
                        rtx_dbug_outs(" has messages");
                        envl_print_deque(pcb_get_message_queue(pcb_table[i]));
                    }
                }

                if(is_envl_deque_empty(process_c1_local_msg_q_addr) == FALSE) {
                    rtx_dbug_outs("C1 has messages on local queue ");
                    envl_print_deque(process_c1_local_msg_q_addr);
                    rtx_dbug_outs("\n\r");
                }
                if(is_envl_deque_empty(process_c2_local_msg_q_addr) == FALSE) {
                    rtx_dbug_outs("C2 has messages on local queue ");
                    envl_print_deque(process_c2_local_msg_q_addr);
                    rtx_dbug_outs("\n\r");
                }
            }
#endif
    
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Going to send message to KCD\n\r");
#endif
            envl* tmp_envl_send = (envl*)request_memory_block();
      
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Requested memory block = ");
            rtx_dbug_out_int16((UINT32)tmp_envl_send);
            rtx_dbug_outs("\n\r");
#endif
      
            envl_set_message_type(tmp_envl_send, KEYBOARD_INPUT);
      
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Set message type as KEYBOARD_INPUT\n\r");
#endif
      
            envl_set_msgtext(tmp_envl_send, tmp_char_in);
      
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Set message text to '");
            rtx_dbug_outs(tmp_char_in);
            rtx_dbug_outs("'\n\r");
            rtx_dbug_outs("[UART] Sending message to KCD\n\r");
#endif
      
            // todo: ensure current_process id is UART_I_PID
            send_message(KCD_PID, tmp_envl_send);
      
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Sent message to KCD\n\r");
#endif

        }
        else if (key_in) { // no new chars received, so time to display message
#ifdef DEBUG_IPROC
            rtx_dbug_outs("[UART] Time to display message os screen\n\r");
#endif

            if(!is_envl_deque_empty(pcb_get_message_queue(current_process))) {

#ifdef DEBUG_IPROC
                rtx_dbug_outs("[UART] Checking for messages\n\r");
#endif
                envl* tmp_envl = (envl*)receive_message(NULL);

#ifdef DEBUG_IPROC
                rtx_dbug_outs("[UART] Received message = ");
                rtx_dbug_out_int16((UINT32)tmp_envl);
                rtx_dbug_outs("\n\r");

                rtx_dbug_outs("[UART] Available Envelopes = ");
                rtx_dbug_out_int10(free_mem_queue_cnt);
                rtx_dbug_outs("\n\r");
#endif

                if(envl_get_message_type(tmp_envl) == CRT_DISPLAY && envl_get_sender_id(tmp_envl) == CRT_DISPLAY_PID) {

#ifdef DEBUG_IPROC
                    rtx_dbug_outs("[UART] Message from CRT\n\r");
#endif

                    char* tmp_msg_out = envl_get_msgtext(tmp_envl);

#ifdef DEBUG_IPROC
                    rtx_dbug_outs("[UART] Message = ");
                    rtx_dbug_outs(tmp_msg_out);
                    rtx_dbug_outs("\n\r");
#endif        	    
                    while(*tmp_msg_out != '\0') {
                        SERIAL1_WD = *tmp_msg_out; // output one char to display
			if (*tmp_msg_out == 0x0a){
                    		rtx_dbug_outs("");
                        	SERIAL1_WD = 0x0d;
	    		}
			++tmp_msg_out;
                    }
                }

#ifdef DEBUG_IPROC
                rtx_dbug_outs("[UART] Done displaying message\n\r");
#endif

                // done displaying message
                release_memory_block((void*)tmp_envl);

#ifdef DEBUG_IPROC
                rtx_dbug_outs("[UART] Messsage envelope released\n\r");
#endif
            }
        }
  
#ifdef DEBUG_IPROC
        rtx_dbug_outs("[UART] Exiting...\n\r");
        rtx_dbug_outs("================================================\n\r");
#endif

        /*  
            current_primitive = RTX_NORMAL;

            // unmask interrupts (disable atomic)
            //asm("move.w #0x2000,%SR");

            asm("TRAP #6"); */
        //asm("move.w #0x2700,%SR");        
        check_schedule();
#ifdef DEBUG_IPROC
        rtx_dbug_outs("[UART] VERY BOTTOM OF UART AFTER TRAP?\n\r");
        rtx_dbug_outs("================================================\n\r");
  
#endif
    }

#ifdef DEBUG_IPROC
    rtx_dbug_outs("[UART] Should not reach here\n\r");
#endif
    check_schedule();
    //    return;
}
