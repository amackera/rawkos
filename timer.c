#include "timer.h"

void c_timer_handler() {
    // assuming the current_process is not an i-process, since i-processes are not supposed to be interruptable

#ifdef DEBUG_IPROC
    rtx_dbug_outs("================================================\n\r");
    rtx_dbug_outs("[TIMER_INTERRUPT_HANDLER] Entered...\n\r");
#endif

/*
        #ifdef DEBUG
//        set_process_priority(KCD_PID, PRIORITY_LEVEL_2);
        set_priority_command_process(KCD_PID, PRIORITY_LEVEL_2);
        rtx_dbug_outs(" Get priority should be 2, returned = ");
        rtx_dbug_out_int16(get_process_priority(KCD_PID));
        rtx_dbug_outs("\n\r");
//        set_process_priority(KCD_PID, PRIORITY_LEVEL_3);
//      rtx_dbug_outs(" Process reset back to priority ");
//      rtx_dbug_out_int16(get_process_priority(KCD_PID));
//      rtx_dbug_outs("\n\r");
        #endif
*/

    //    process_switch(get_pcb(TIMER_I_PID));

    // Attempt to Context switch 
    current_primitive = CONTEXT_SWITCH_TIMER;
    asm("TRAP #6");	

}

void timer_i_process() {

    char cur_time[32];

    while (1) {

        // mask interrupts (enable atomic)
        asm("move.w #0x2700,%SR");


#ifdef DEBUG_IPROC
        rtx_dbug_outs("================================================\n\r");
        rtx_dbug_outs("[TIMER] Entered...\n\r");
	print_ready_queue();
	print_blocked_on_receive_queue();
#endif
    
        currtime += CLK_UNIT; // has units of second
	delaytime += CLK_UNIT;

#ifdef DEBUG_IPROC
        rtx_dbug_outs("[TIMER] currtime = ");
        rtx_dbug_out_int10(currtime);
        rtx_dbug_outs("\n\r");
#endif

        TIMER0_TER = 2;
        if(!is_envl_deque_empty(delay_send_queue)) {

#ifdef DEBUG_IPROC
            rtx_dbug_outs("[TIMER] Something on delay_send_queue\n\r");
#endif

            // check if it is time to send the message
            envl* tmp_envl = envl_peek_head(delay_send_queue);
            if(tmp_envl != NULL) {
                // implies something on delay_send_queue
                SYSTIME dtime = envl_get_delivery_time(tmp_envl);

#ifdef DEBUG_IPROC
                rtx_dbug_outs("[TIMER] delivery time = ");
                rtx_dbug_out_int10(dtime);
                rtx_dbug_outs("\n\r");
#endif

                if(dtime <= delaytime) {
                    // is time to deliver
                    tmp_envl = dequeue_from_delay_send_queue();

#ifdef DEBUG_IPROC
                    rtx_dbug_outs("[TIMER] destination process = ");
                    rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_destination_pid(tmp_envl))));
                    rtx_dbug_outs("\n\r");
#endif
		    delaySend = TRUE; //used to signal to send_message() to not change the original sender_pid
                    send_message(envl_get_destination_pid(tmp_envl), (void*)tmp_envl);
                }
            }
            else {
#ifdef DEBUG_IPROC
                rtx_dbug_outs("[TIMER] Nothing on delay_send_queue\n\r");
#endif
                // nothing to deliver, so exit
            }
        }
        if(clock_state == TRUE){
            // Incrememnt timer in WC
            envl* envl_wall_clock = (envl*)request_memory_block();
            envl_set_sender(envl_wall_clock,TIMER_I_PID);
            send_message(WALL_CLOCK_PID,envl_wall_clock);

            //int j = 0; Reusing LSD for J	


            /*            

            //cur_time[5] = 0x7B; // {
            cur_time[5] = 0x30; // 0
            //	cur_time[6] = 0x39; // 9
            cur_time[6] = 0x3B; //;
            cur_time[7] = 0x32; // 2
            cur_time[8] = 0x34; // 4
            cur_time[9] = 0x48; // H
            
            cur_time[10] = 0x1B; //Esc
            cur_time[11] = 0x5B; //[
            cur_time[12] = 0x31; //1
            cur_time[13] = 0x4A; // K clear //cur_time[2] = 0x73; //s
            
            cur_time[14] = 0x1B; //Esc
            cur_time[15] = 0x5B; //[
            cur_time[16] = 0x30; //0
            cur_time[17] = 0x4A; // K clear //cur_time[2] = 0x73; //s
            
            while(time!=0){ 
                lsd = time % 10;
                tmp_time[i] = "0123456789"[lsd];
                time = time / 10; //remove current ones digit
                i++;
            }
            lsd = 18;
            while(i > 0){
                cur_time[lsd] = tmp_time[i-1];
                i--;
                lsd++;
            }
            cur_time[lsd] = 0x0d; // CR 
            cur_time[++lsd] = 0x1B; //Esc
            cur_time[++lsd] = 0x5B; //[
            cur_time[++lsd] = 0x75; //restore
#ifdef DISPLAY_TIMER
            rtx_dbug_outs("[UART] Requested memory block = ");
            rtx_dbug_out_int16((UINT32)tmp_envl_send);
            rtx_dbug_outs("\n\r");
            rtx_dbug_outs("The Message of the envelope to CRT is \n\r");
            rtx_dbug_out_int16((UINT16)tmp_time);
            rtx_dbug_outs("\n\r");
#endif
            envl_set_message_type(tmp_envl_send, CRT_DISPLAY);
	envl_set_msgtext(tmp_envl_send, cur_time);
	send_message(CRT_DISPLAY_PID, tmp_envl_send);
            */
        }
                    
#ifdef DEBUG_IPROC
        rtx_dbug_outs("[TIMER] Exiting...\n\r");
        rtx_dbug_outs("================================================\n\r");
#endif
  

        //        current_primitive = RTX_NORMAL;

        // unmask interrupts (disable atomic)
        //asm("move.w #0x2000,%SR");

        //asm("trap #6");
        check_schedule();
    }

#ifdef DEBUG_IPROC
    rtx_dbug_outs("[TIMER] Should not reach here\n\r");
#endif

    /*
    current_primitive = RTX_NORMAL;

    // unmask interrupts (disable atomic)
    //asm("move.w #0x2000,%SR");

    asm("trap #6");
    */
    //    return;
    check_schedule();
    }
