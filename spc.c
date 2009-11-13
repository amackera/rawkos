#include "spc.h"

int set_priority_command_process() {
    register_command(*"C");
    envl* tmp_envl = NULL;

    char* msgtext = NULL;

    int j = 0;
    int i = 0;
    const int buffsize = 64;
    char tmp_param[buffsize];

    int tmp_pid = INVALID_PID;
    int tmp_newpri = INVALID_PRIORITY_LEVEL;

    while(1) {
        tmp_envl = (envl*)receive_message(NULL); // todo: should check for valid pid
        // assumming only KCD sends messages to SPC

        // assuming command string has the format:
        // "%C" SPACE+ PID SPACE+ NEW_PRIORITY SPACE*

        msgtext = tmp_envl->msgtext;
#ifdef DBSPC
            rtx_dbug_outs("[SPC] msgtext = '");
            rtx_dbug_outs(msgtext);
            rtx_dbug_outs("'\n\r");
#endif

        j = 0;
        i = 2; // since first two characters should be "%C"
        while(msgtext[i] == 0x20) {i++;} // ' '
        while(msgtext[i] != 0x20) {
            tmp_param[j++] = msgtext[i++];
        }
        tmp_param[j] = '\0';
        tmp_pid = atoi(tmp_param);
        // check if valid pid later
        j = 0; // reset buffer by overwriting old data
        while(msgtext[i] == 0x20) {i++;} // ' '
        while(msgtext[i] != 0x20 && msgtext[i] != '\0') {
            tmp_param[j++] = msgtext[i++];
        }
        tmp_param[j] = '\0';
        tmp_newpri = atoi(tmp_param);
        // check if valid priority later

        if(!is_valid_pid(tmp_pid)
           || !is_valid_priority(tmp_newpri)
           || tmp_newpri == PRIORITY_LEVEL_4  // can't change NULL process priority
           ) {
#ifdef DBSPC
            rtx_dbug_outs("[SPC] Invalid priority, non-existing process, xor changing Null process priority\n\r");
            rtx_dbug_outs("[SPC] wrong pid = ");
            rtx_dbug_out_int10(tmp_pid);
            rtx_dbug_outs("\n\r");
            rtx_dbug_outs("[SPC] wrong priority = ");
            rtx_dbug_out_int10(tmp_newpri);
            rtx_dbug_outs("\n\r");
#endif
            release_memory_block(tmp_envl);
        }
        else {
#ifdef DBSPC
            rtx_dbug_outs("[SPC] SUCCESS\n\r");
            rtx_dbug_outs("[SPC] new pid = ");
            rtx_dbug_out_int10(tmp_pid);
            rtx_dbug_outs("\n\r");
            rtx_dbug_outs("[SPC] new priority = ");
            rtx_dbug_out_int10(tmp_newpri);
            rtx_dbug_outs("\n\r");
#endif
            set_process_priority(tmp_pid, tmp_newpri);
            release_memory_block(tmp_envl);
        }
    }
}
