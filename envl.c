#include "envl.h"

// Responsibility for checking if in_envl == NULL left to the caller
// This function does not clear the next and prev pointers because they are not part of the envelope, per-se; they are part of the envl_deque.
int envl_clear(envl* in_envl) {
    if(in_envl != NULL) {
        in_envl->sender_pid = INVALID_PID;
        in_envl->destination_pid = INVALID_PID;
        in_envl->msg_type = INVALID_MESSAGE_TYPE;
        in_envl->sent_time = 0; // -1; // to reduce space in print out
        in_envl->delivery_time = 0; // -1;
        in_envl->reserved[0] = '\0';
        in_envl->msgtext[0] = '\0';
    }
    return RTX_SUCCESS;
}

int envl_set_sender(envl* env, PROCESS_ID sender_pid) {
    if(!is_valid_pid(sender_pid) || env == NULL) {
        return RTX_ERROR;
    }

    env->sender_pid = sender_pid;
    return RTX_SUCCESS;
}

int envl_set_destination(envl* env, PROCESS_ID destination_pid) {
    if(!is_valid_pid(destination_pid) || env == NULL) {
        return RTX_ERROR;
    }

    env->destination_pid = destination_pid;
    return RTX_SUCCESS;
}

int envl_set_sent_time(envl* in_envl, SYSTIME in_stime) {
    if(in_envl == NULL || !is_valid_systime(in_stime)) {
        return RTX_ERROR;
    }
    in_envl->sent_time = in_stime;
    return RTX_SUCCESS;
}

int envl_set_delivery_time(envl* in_envl, SYSTIME in_dtime) {
    if(in_envl == NULL || !is_valid_systime(in_dtime)) {
        return RTX_ERROR;
    }
    in_envl->delivery_time = in_dtime;
    return RTX_SUCCESS;
}

int envl_set_message_type(envl* env, MESSAGE_TYPE msg_type) {
    if(env == NULL || !is_valid_message_type(msg_type)) {
        return RTX_ERROR;
    }

    env->msg_type = msg_type;
    return RTX_SUCCESS;
}







PROCESS_ID envl_get_sender_id(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_ERROR;
    }

    return in_envl->sender_pid;
}

MESSAGE_TYPE envl_get_message_type(envl* in_envl) {
    if(in_envl == NULL) {
    	return RTX_ERROR;
    }

    return in_envl->msg_type;
}

PROCESS_ID envl_get_destination_pid(envl* in_envl) {
    if(in_envl == NULL) {
    	return RTX_ERROR;
    }

    return in_envl->destination_pid;
}

SYSTIME envl_get_sent_time(envl* in_envl) {
    if(in_envl == NULL) {
    	return RTX_ERROR;
    }

    return in_envl->sent_time;
}

SYSTIME envl_get_delivery_time(envl* in_envl) {
    if(in_envl == NULL) {
    	return RTX_ERROR;
    }

    return in_envl->delivery_time;
}

char* envl_get_msgtext(envl* in_envl) {
    if(in_envl == NULL) {
    	return NULL;
    }

    return in_envl->msgtext;
}

int envl_print(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_ERROR;
    }

    rtx_dbug_outs("sender: ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(in_envl->sender_pid)));
    rtx_dbug_outs("; ");
    rtx_dbug_outs("dest: ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(in_envl->destination_pid)));
    rtx_dbug_outs("; ");
    rtx_dbug_outs("msgtype: ");
    rtx_dbug_outs(get_message_type_name(in_envl->msg_type));
    rtx_dbug_outs("; ");
    rtx_dbug_outs("sent: ");
    rtx_dbug_out_int10(in_envl->sent_time);
    rtx_dbug_outs("; ");
    rtx_dbug_outs("deliv: ");
    rtx_dbug_out_int10(in_envl->delivery_time);
    rtx_dbug_outs("; ");
    rtx_dbug_outs("msgtext: \"");
    rtx_dbug_outs(in_envl->msgtext);
    rtx_dbug_outs("\"\n\r");

    return RTX_SUCCESS;
}
