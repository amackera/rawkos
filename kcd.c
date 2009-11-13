#include "kcd.h"

// assuming space in between command char and each parameter
int check_parameters(char in_cmd, char* in_param) {
    if(in_cmd == '\0') {
        return RTX_ERROR;
    }

    const int buffsize = 64;
    char tmp_param[buffsize];

    // assume in_cmd in cmd_table
    if(in_cmd == 0x57) {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] W -- wall clock \n\r");
#endif
        // W
        // keyboard command
        if(in_param[0] == 0x53) {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] S -- set \n\r");
#endif
            // S
            // sets current wall clock time to param
            int j = 0;
            int i = 1;
            while(in_param[i] == 0x20) {i++;}
            while(in_param[i] != 0x20 && in_param[i] != '\0') {
                tmp_param[j++] = in_param[i++];
            }
            tmp_param[j] = '\0';
            if(j > 8) {
                // though command may be correct, there is garbage appended at the end
                return RTX_ERROR;
            }
            // format should be hh:mm:ss
            // and should be a "standard" 24 hour time
            // todo: check ^ and v
            // should check if at null in tmp_param
            if((tmp_param[0] >= 0x30) && (tmp_param[0] <= 0x32) &&
               (tmp_param[1] >= 0x30) && (tmp_param[1] <= 0x34) &&
               (tmp_param[2] == 0x3a) && /* : */
               (tmp_param[3] >= 0x30) && (tmp_param[3] <= 0x35) &&
               (tmp_param[4] >= 0x30) && (tmp_param[4] <= 0x39) &&
               (tmp_param[5] == 0x3a) &&
               (tmp_param[6] >= 0x30) && (tmp_param[6] <= 0x35) &&
               (tmp_param[7] >= 0x30) && (tmp_param[7] <= 0x39)) {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] SUCCESS Wall Clock Time parsed correctly \n\r");
#endif
                return RTX_SUCCESS;
            }
            else {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] ERROR In Wall Clock Time format \n\r");
#endif
                return RTX_ERROR; // hh:mm:ss time format malformed
            }
        }
        else if (in_param[0] == 0x54) {
            // T
            // terminate display
            int i = 1;
            while(in_param[i] == 0x20) {i++;}
            if(in_param[i] != '\0') {
                // garbage after valid input
                return RTX_ERROR;
            }
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] T --> SUCCESS Wall Clock Terminated \n\r");
#endif
            return RTX_SUCCESS;
        }
        else {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] ERROR Not a recognized 'W' command \n\r");
#endif
            return RTX_ERROR; // not a recognized mode for the 'W' command
        }
    }
    else if (in_cmd == 0x43) {
        // C
        // set priority command
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] C -- set priority command \n\r");
#endif
        int j = 0; // will count number of non ' ' chars
        int i = 0;

        while(in_param[i] == 0x20) {i++;} // ' '
        while(in_param[i] != 0x20) {
            tmp_param[j++] = in_param[i++];
        }
        tmp_param[j] = '\0';
        int tmp_pid = atoi(tmp_param);
        if(!is_valid_pid(tmp_pid)) {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] ERROR Invalid PID for set priority command \n\r");
#endif
            return RTX_ERROR; // invalid pid
        }
        j = 0; // reset buffer by overwriting old data

        while(in_param[i] == 0x20) {i++;} // ' '
        while(in_param[i] != 0x20 && in_param[i] != '\0') {
            tmp_param[j++] = in_param[i++];
        }
        tmp_param[j] = '\0';
        int tmp_newpri = atoi(tmp_param);
        if(!is_valid_priority(tmp_newpri)) {
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] ERROR Invalid priority for set priority command \n\r");
#endif
            return RTX_ERROR; // invalid priority
        }
#ifdef DBKCD
    rtx_dbug_outs("[CHECK_PARAMETERS] SUCCESS Valid Set Priority Command parameters \n\r");
#endif
        return RTX_SUCCESS;
    }
    else if(in_cmd == 0x5a) {
        // Z
        // process A1
        int i = 0;
        while(in_param[i] == 0x20) {i++;}
        if(in_param[i] != '\0') {
            return RTX_ERROR; // This command is parameterless
        }
        return RTX_SUCCESS;
    }
    else if(in_cmd == 0x59) {
        // Y
        // process A2
        int i = 0;
        while(in_param[i] == 0x20) {i++;}
        if(in_param[i] != '\0') {
            return RTX_ERROR; // This command is parameterless
        }
        return RTX_SUCCESS;
    }
    else if(in_cmd == 0x58) {
        // X
        // process A3
        int i = 0;
        while(in_param[i] == 0x20) {i++;}
        if(in_param[i] != '\0') {
            return RTX_ERROR; // This command is parameterless
        }
        return RTX_SUCCESS;
    }
    else {
        return RTX_ERROR; // Not a recognized command
    }
}

int print_cmd_table() {
    int i = 0;
    char tmp_buf[2] = {0,0};
    for(; i<cmd_table_cnt; i++) {
        tmp_buf[0] = cmd_table[i].cmd;
        rtx_dbug_outs("cmd: ");
        rtx_dbug_outs(tmp_buf);
        rtx_dbug_outs("; pid: ");
        rtx_dbug_out_int10(cmd_table[i].pid);
        rtx_dbug_outs("\n\r");
    }
}

BOOL is_command_char(char in_cmd) {
    return in_cmd == 0x25 /* '%' */ ? TRUE : FALSE;
}

/*
  int verify_command(char* in_cmd) { // '%' <cmd_char>
  PROCESS_ID tmp_pid = search_command(in_cmd);
  if(tmp_pid == INVALID_PID) {
  return RTX_ERROR;
  }
  else {
  return RTX_SUCCESS;
  }
  }
*/

// assume command identifier is only a single char, and everything after it are parameters
PROCESS_ID search_command(char in_cmd) {
    int i = 0;
    for(; i<cmd_table_cnt; i++) {
        if(cmd_table[i].cmd == in_cmd) {
            return cmd_table[i].pid;
        }
    }
    return RTX_ERROR;
}

int register_command(char in_cmd) {

#ifdef DBKCD
    rtx_dbug_outs("[REGISTER_COMMAND] ");
    rtx_dbug_out_int10(cmd_table_cnt);
    rtx_dbug_outs(" commands registered\n\r");
#endif

    char tmp_str[2] = {'\0', '\0'};
    tmp_str[0] = in_cmd;

    process_registration(in_cmd, pcb_get_pid(current_process));

    if(search_command(in_cmd) != RTX_ERROR) {
#ifdef DBKCD
        rtx_dbug_outs("[REGISTER_COMMAND] Command '");
        rtx_dbug_outs(tmp_str);
        rtx_dbug_outs("' already registered\n\r");
#endif
        return RTX_SUCCESS; // command already registered
    }

#ifdef DBKCD
    rtx_dbug_outs("[REGISTER_COMMAND] Requesting envelope to send registration \n\r");
#endif

    envl* tmp_envl = (envl*)request_memory_block();

#ifdef DBKCD
    rtx_dbug_outs("[REGISTER_COMMAND] Sending message to register command '");
    rtx_dbug_outs(tmp_str);
    rtx_dbug_outs("'\n\r");
#endif

    envl_set_message_type(tmp_envl, KCD_REGISTER);
    envl_set_msgtext(tmp_envl, tmp_str);
    
    return send_message(KCD_PID, (void*)tmp_envl);
}

int process_registration(char in_cmd, PROCESS_ID in_pid) {
    if(!is_valid_pid(in_pid)) {
        return RTX_ERROR;
    }
  
    // check if command already in table; otherwise add. If it is already in the table, re-register
    int i = 0;
    for(; i<cmd_table_cnt; i++) {
        if(cmd_table[cmd_table_cnt].cmd == in_cmd) {
            // it has already been registered
            // so re-register
            cmd_table[cmd_table_cnt].cmd = in_cmd;
            cmd_table[cmd_table_cnt].pid = in_pid;
            return RTX_SUCCESS;
        }
    }
  
    if(i >= MAX_CMD_COUNT) {
        return RTX_ERROR; // need to allocate more space for cmd_table
    }
  
    cmd_table[i].cmd = in_cmd;
    cmd_table[i].pid = in_pid;
    ++cmd_table_cnt;
    return RTX_SUCCESS;
}

void keyboard_command_decoder() {
#ifdef DBKCD
    rtx_dbug_outs("============================================\n\r");
    rtx_dbug_outs("[KEYBOARD_COMMAND_DECODER] Entering...\n\r");
#endif
    const int buffsize = ENVL_MSGTEXT_SIZE;
    char buffer[buffsize]; // contains user input // me: might need to do buffsize +1 here and have buffer[buffsize] = '\0' always (which no one should know about) // maybe want to do the same for envl.msgtext[] (although this might throw off other calculations)
  
    int buffcnt = 0; // buffer count
    while(1) {
#ifdef DBKCD
        rtx_dbug_outs("[KCD] Calling receive_message()\n\r");
#endif
        envl* tmp_envl = (envl*)receive_message(NULL);
#ifdef DBKCD
        rtx_dbug_outs("[KCD] tmp_envl = ");
        rtx_dbug_out_int16((UINT32)tmp_envl);
        rtx_dbug_outs("\n\r");		    
#endif
        // register commands
        if(tmp_envl != NULL && envl_get_message_type(tmp_envl) == KCD_REGISTER && envl_get_sender_id(tmp_envl) != UART_I_PID) {
            char* tmp_msgtext = envl_get_msgtext(tmp_envl);
            // tmp_msgtext should be one character only
            process_registration(*tmp_msgtext, envl_get_sender_id(tmp_envl));
#ifdef DBKCD
            rtx_dbug_outs("[KCD] sender = ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(envl_get_sender_id(tmp_envl))));
            rtx_dbug_outs("\n\r");
            rtx_dbug_outs("[KCD] registration char = ");
            rtx_dbug_outs(tmp_msgtext);
            rtx_dbug_outs("\n\r");
            /*
            rtx_dbug_outs("[KCD] # cmd in table = ");
            rtx_dbug_out_int10(cmd_table_cnt);
            rtx_dbug_outs("\n\r");
            */
            //rtx_dbug_outs("[KCD] Printing cmd_table");
            //print_cmd_table();
            //rtx_dbug_outs("\n\r");
#endif
      
            // done reading message
            release_memory_block((void*)tmp_envl);
        }
    
        // keyboard input
        if(envl_get_sender_id(tmp_envl) == UART_I_PID) {
            char* tmp_msgtext = envl_get_msgtext(tmp_envl);
            char tmp_value = *tmp_msgtext; // is 1 char
      
#ifdef DBKCD
            rtx_dbug_outs("[KCD] keyboard input = '");
            rtx_dbug_outs(envl_get_msgtext(tmp_envl));
            rtx_dbug_outs("'\n\r");
            rtx_dbug_outs("[KCD] Now sending to CRT display...\n\r");
#endif
      
            // set the MESSAGE_TYPE of the envl to CRT_DISPLAY 
            envl_set_message_type(tmp_envl, CRT_DISPLAY);			
            // send the envelope to the CRT to be displayed
            send_message(CRT_DISPLAY_PID,tmp_envl);
      
            // todo: move
            /*
                buffer[buffcnt] = tmp_value;
                buffcnt++;
                buffer[buffcnt] = '\0'
             */
            // up here since a character should always go into the buffer (and since this reduces the number of places where buffering occurs (thereby reducing amount of places needing checking))
            // todo: also, add a check to see if buffcnt >= ENVL_MSGTEXT_SIZE and null terminate immediately instead if so (assuming buffer[ENVL_MSGTEXT_SIZE+1])

            if(tmp_value == 0x0a) { // LF
                //buffer[buffcnt] = tmp_value;
                //buffcnt++;
                buffer[buffcnt] = '\0';
        	
#ifdef DBKCD
                rtx_dbug_outs("[KCD] buffered input = '");
                rtx_dbug_outs(buffer);
                rtx_dbug_outs("'\n\r");
#endif
        
                // 1+ chars in buffer[]
                // first two chars should be '%' <cmd_char>
                char tmp_percent_char = buffer[0];
                char tmp_cmd_char = buffer[1];
                char* tmp_param = &buffer[2];
        
                PROCESS_ID tmp_pid = search_command(tmp_cmd_char);
        
                if(is_command_char(tmp_percent_char) == FALSE || // not a '%'
                   tmp_pid == RTX_ERROR) { // ie. not registered

                    clear_buffer(buffer, buffsize);
		    buffcnt = 0;
            //#ifdef DBKCD

                    rtx_dbug_outs("[KCD] Not a command or not registered\n\r");

                    //#endif

                }
                else {
                    // tmp_percent_char == '%'
                    // tmp_cmd_char == <cmd_char>
          
                    int res = check_parameters(tmp_cmd_char, tmp_param);
                    if(res == RTX_ERROR) {
                        clear_buffer(buffer, buffsize);
            		buffcnt = 0;
//#ifdef DBKCD
                        rtx_dbug_outs("[KCD] Command char or parameters malformed\n\r");
//#endif
                    }
                    else {
                        envl* tmp_envl2 = (envl*)request_memory_block();
                        char* tmp_msgtext = envl_get_msgtext(tmp_envl2);

#ifdef DBSPC
                rtx_dbug_outs("[KCD] buffered inpu2t = '");
                rtx_dbug_outs(buffer);
                rtx_dbug_outs("'\n\r");
#endif            
                        copy_buffer(buffer, tmp_msgtext, buffsize);
#ifdef DEBUG
                rtx_dbug_outs("[KCD] after copy = '");
                rtx_dbug_outs(buffer);
                rtx_dbug_outs("'\n\r");
#endif            
            
                        clear_buffer(buffer, buffsize);
            		buffcnt = 0;
#ifdef DBSPC
                        rtx_dbug_outs("[KCD] Sending '");
                        rtx_dbug_outs(tmp_msgtext);
                        rtx_dbug_outs("' to ");
                        rtx_dbug_outs(pcb_get_name(peek_pcb(tmp_pid)));
                        rtx_dbug_outs("\n\r");
#endif
                  
              	          send_message(tmp_pid,tmp_envl2);
                    }
                }
            }
            else { // if any other character is received then add to the buffer
		if (buffcnt < (ENVL_MSGTEXT_SIZE-1)){ 
                    buffer[buffcnt] = tmp_value;
                    buffcnt++;
                    buffer[buffcnt] = '\0'; // assuming buffcnt < ENVL_MSGTEXT_SIZE (if buffer[ENVL_MSGTEXT_SIZE+1]) // this way, the buffer is always null terminated
#ifdef DBKCD
                    rtx_dbug_outs("[KCD] buffer =");
                    rtx_dbug_outs(buffer);
                    rtx_dbug_outs("\n\r");
#endif
		}

#ifdef DBKCD
                rtx_dbug_outs("[KCD] Character placed in buffer\n\r");
#endif
            }
        }
    }
}
