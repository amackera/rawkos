#include "msglog.h"

BOOL msglog_is_pos_in_range(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL) {
        return RTX_ERROR;
    }

    int cnt = 0;
    int currpos = in_msglog->startpos;
    while(cnt < in_msglog->entry_cnt) {
        if(currpos == in_pos) {
            return TRUE;
        }
        currpos = msglog_get_nextpos(currpos);
    }
    return FALSE;
}

PROCESS_ID msglog_get_sender_pid(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL || !msglog_is_pos_in_range(in_msglog, in_pos)) {
        return RTX_ERROR;
    }
    return (in_msglog->msglog_entry[in_pos]).sender_pid;
}

PROCESS_ID msglog_get_destination_pid(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL || !msglog_is_pos_in_range(in_msglog, in_pos)) {
        return RTX_ERROR;
    }
    return (in_msglog->msglog_entry[in_pos]).destination_pid;
}

MESSAGE_TYPE msglog_get_msg_type(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL || !msglog_is_pos_in_range(in_msglog, in_pos)) {
        return RTX_ERROR;
    }
    return (in_msglog->msglog_entry[in_pos]).msg_type;
}

SYSTIME msglog_get_transaction_time(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL || !msglog_is_pos_in_range(in_msglog, in_pos)) {
        return RTX_ERROR;
    }
    return (in_msglog->msglog_entry[in_pos]).transaction_time;
}

char* msglog_get_msgtext(msglog* in_msglog, int in_pos) {
    if(in_msglog == NULL || !msglog_is_pos_in_range(in_msglog, in_pos)) {
        return NULL;
    }
    return (in_msglog->msglog_entry[in_pos]).msgtext;
}

int msglog_get_nextpos(int in_currpos) {
    return (in_currpos + 1) % MSGLOG_SIZE;
}

int msglog_save_transaction(msglog* in_msglog, envl* in_envl) {
    if(in_msglog == NULL || in_envl == NULL) {
        return RTX_ERROR;
    }

    int nextpos = msglog_get_nextpos(in_msglog->startpos);
    ++(in_msglog->entry_cnt);
    if(in_msglog->entry_cnt > MSGLOG_SIZE) {
        // overwrite least recent entry
        in_msglog->entry_cnt = MSGLOG_SIZE;
        in_msglog->startpos = msglog_get_nextpos(in_msglog->startpos);
    }

    (in_msglog->msglog_entry[nextpos]).sender_pid = in_envl->sender_pid;
    (in_msglog->msglog_entry[nextpos]).destination_pid = in_envl->destination_pid;
    (in_msglog->msglog_entry[nextpos]).msg_type = in_envl->msg_type;
    (in_msglog->msglog_entry[nextpos]).transaction_time = currtime; // me: or should this be in_envl->sent_time ?
    int i = 0;
    for(; i<MSGLOG_SAMPLE_SIZE; i++) {
        (in_msglog->msglog_entry[nextpos]).msgtext[i] = in_envl->msgtext[i];
    }
    (in_msglog->msglog_entry[nextpos]).msgtext[i] = '\0';
    return RTX_SUCCESS;
}

int msglog_print(msglog* in_msglog) {
    if(in_msglog == NULL) {
        return RTX_ERROR;
    }

    int currpos = in_msglog->startpos;
    int printcnt = 0;
    while(printcnt < in_msglog->entry_cnt) {
        // todo: print
        rtx_dbug_outs("Sender  = ");
        rtx_dbug_outs(pcb_get_name(peek_pcb(msglog_get_sender_pid(in_msglog, currpos))));
        rtx_dbug_outs("\n\r");
        rtx_dbug_outs("Dest    = ");
        rtx_dbug_outs(pcb_get_name(peek_pcb(msglog_get_destination_pid(in_msglog, currpos))));
        rtx_dbug_outs("\n\r");
        rtx_dbug_outs("Msgtype = ");
        rtx_dbug_outs(get_message_type_name(msglog_get_msg_type(in_msglog, currpos)));
        rtx_dbug_outs("\n\r");
        rtx_dbug_outs("Transaction time = ");
        rtx_dbug_out_int10(msglog_get_transaction_time(in_msglog, currpos));
        rtx_dbug_outs("\n\r");
        rtx_dbug_outs("Msgtext = ");
        rtx_dbug_outs(msglog_get_msgtext(in_msglog, currpos));
        rtx_dbug_outs("\n\r");
        currpos = msglog_get_nextpos(currpos);
        ++printcnt;
    }

    return RTX_SUCCESS;
}
