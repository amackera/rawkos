#include "wall_clock.h"

void wall_clock_display_process() {
    register_command(*"W");

    const int timesize = 8;
    char timestr[timesize+1];
    char vt100_msg[64];
    //    char vt100_footer[32];
    while(1) {
        //int tmp_pid = pcb_get_pid(current_process);
        envl* tmp_envl = (envl*)receive_message(NULL); // receive message from anywhere

        if((envl_get_sender_id(tmp_envl)) && (clock_state == TRUE)) { //MSG is from timer

#ifdef DEBUG_WALLCLOCK
            rtx_dbug_outs("[WALLCLOCK] Sending VT100 headers\n\r");
#endif
            int_to_time(currtime,timestr);

#ifdef DEBUG_WALLCLOCK
            rtx_dbug_outs("[WALLCLOCK] Updating CLOCK to = ");
            rtx_dbug_outs(timestr);
            rtx_dbug_outs("\n\r");
#endif

            envl* envl_out_clk = (envl*)request_memory_block();
            envl_set_message_type(envl_out_clk,CRT_DISPLAY);
            envl_set_msgtext(envl_out_clk,timestr);
            send_message(CRT_DISPLAY_PID,envl_out_clk);

        }

        // assuming command and parameters well-formed
        // assuming command string has the format:
        // "%W" ("S" hh ":" mm ":" ss SPACE* | "T" SPACE*)
        char* tmp_msgtext = envl_get_msgtext(tmp_envl);

#ifdef DEBUG_WALLCLOCK
        rtx_dbug_outs(tmp_msgtext);
#endif

        char tmp_scmd = tmp_msgtext[2];
        if(tmp_scmd  == 0x53) { // S
            int i = 0;
            for(; i<timesize; i++) {
                timestr[i] = tmp_msgtext[i+3];
            }
            timestr[i] = '\0';

            currtime = time_to_int(timestr);

            clock_state = TRUE;

            //release_memory_block((void*)tmp_envl);

#ifdef DEBUG_WALLCLOCK
            rtx_dbug_outs("Forwarding to wall_clock \n\r");
#endif

            // Forward msg to CRT display but change msgtext to be text of message
            envl_set_msgtext(tmp_envl, timestr); // add timestr to message
            envl_set_message_type(tmp_envl, CRT_DISPLAY);
            send_message(CRT_DISPLAY_PID, tmp_envl);

            /*
            envl* tmp_envl2 = (envl*)request_memory_block();

            enque_onto_blocked_on_receive_queue(current_process, pcb_get_priority(current_process));
            delayed_send(tmp_pid, tmp_envl2, 1); // delay 1 second
            */
            /*
              rtx_dbug_outs("In the wall clock. Setting to TRUE \n\r");
              currtime = (int)timestr[0] * 10 + (int)timestr[1];
            */
        }
        else if(tmp_scmd  == 0x54) { // T
            // todo: terminate display
            /*
            // CLEAR
            rtx_dbug_outs("In the wall clock. Setting to FALSE \n\r");
            */

            clock_state = FALSE;
            
            release_memory_block((void*)tmp_envl);
        }
        else {
#ifdef DEBUG
            rtx_dbug_outs("[WALL CLOCK] Message text not recognized by wall clock\n\r");
#endif
            release_memory_block((void*)tmp_envl);
        }
    }
}
