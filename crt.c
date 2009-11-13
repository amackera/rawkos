#include "crt.h"

void crt_display() {
    /*
	envl*  message_display;
	char* message_display_start;

    char* vt100_msg;
    char* vt100_msg_start;

    int i = 0;

    vt100_msg_start = '\0';

    char_cat(ESC, vt100_msg_start);
    str_cat("[1;1H", vt100_msg_start);
    str_cat("RAWKOS", vt100_msg_start);
    
    char_cat(ESC, vt100_msg_start);
    str_cat("[2;1H", vt100_msg_start);
	for (i = 0; i < 80; i++) char_cat('-', vt100_msg_start);
	char_cat(ESC, vt100_msg_start);
	str_cat("[23;1H", vt100_msg_start);
	for (i = 0; i < 80; i++) char_cat('-', vt100_msg_start);
	// sets up scroll region
	char_cat(ESC, vt100_msg_start);
	str_cat("[3;22r", vt100_msg_start);
	// move cursor to line 3
	char_cat(ESC, vt100_msg_start);
	str_cat("[3;1H", vt100_msg_start);
    
    while (vt100_msg_start[i] != '\0')
	{
        
		message_display = (envl*)request_memory_block();

		*message_display_start = *vt100_msg_start;
		send_message (UART_IPROCESS_ID, message_display); // we are sending character by character out to the UART process
		SERIAL1_IMR = 3;
        i++;
        }    */

    //splash_screen();

    while(1) {

#ifdef DEBUG
        rtx_dbug_outs("[CRT_DISPLAY] Entering...\n\r");
        rtx_dbug_outs("[CRT_DISPLAY] Calling receive_message()\n\r");
#endif
        envl* tmp_envl = (envl*)receive_message(NULL);

#ifdef DEBUG
        rtx_dbug_outs("[CRT_DISPLAY] Received message envelope = ");
        rtx_dbug_out_int16((UINT32)tmp_envl);
        rtx_dbug_outs("\n\r");
#endif
    
        // ensure message received intended for CRT_DISPLAY
        if(tmp_envl != NULL && envl_get_message_type(tmp_envl) == CRT_DISPLAY) {
            // send message to UART to display it on the screen

#ifdef DEBUG_CRT
            rtx_dbug_outs("[CRT_DISPLAY] Sending message to UART\n\r");
#endif
            
            char* msg = envl_get_msgtext(tmp_envl);
            char* msg_start = msg;
            while (*msg_start != 0x00) {
                char tmp_array[2];
                tmp_array[0] = *msg_start;
                tmp_array[1] = 0x00;
                envl* forward = (envl*)request_memory_block();
                envl_set_msgtext(forward, tmp_array);
                envl_set_message_type(forward,CRT_DISPLAY);
                send_message(UART_I_PID,forward);
                SERIAL1_IMR = 0x03;
                msg_start++;
            }
            
#ifdef DEBUG_CRT
            rtx_dbug_outs("[CRT_DISPLAY] REACHED NULL TERMINATION\n\r");
#endif
		release_memory_block((void*)tmp_envl);
        	//    release_memory_block(msg);
            
            //send_message(UART_I_PID,tmp_envl);
            
#ifdef DEBUG_CRT
            rtx_dbug_outs("[CRT_DISPLAY] Message sent to UART\n\r");
#endif
      
	    // enable UART interrupt mask register
            SERIAL1_IMR = 0x03;
      
        }
        else {
            // release anything that is not a CRT_DISPLAY message
            release_memory_block(tmp_envl);

#ifdef DEBUG_CRT
            rtx_dbug_outs("[CRT_DISPLAY] Memory block released\n\r");
#endif

        }
    }
}

/*
void splash_screen() {
    
    envl* splash_envl = (envl*)request_memory_block();
    char* splash_msg;
    splash_msg[0] = '\0';
    str_cat("RAWKOS",splash_msg);
    
    envl_set_message_type(splash_msg, CRT_DISPLAY);
    envl_set_msgtext(splash_envl,splash_msg);

    send_message(CRT_DISPLAY_PID,splash_envl);

}
*/
