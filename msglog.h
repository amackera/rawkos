#ifndef MSGLOG_H
#define MSGLOG_H

#include "globalvar.h"
#include "envl.h"
#include "func.h"

#define MSGLOG_SIZE 8
#define MSGLOG_SAMPLE_SIZE 16

// circular log buffer
typedef struct {
    struct {
        PROCESS_ID sender_pid;
        PROCESS_ID destination_pid;
        MESSAGE_TYPE msg_type;
        SYSTIME transaction_time;
        char msgtext[MSGLOG_SAMPLE_SIZE+1];
    } msglog_entry[MSGLOG_SIZE];
    int entry_cnt; // init to 0
    int startpos; // init to 0
} msglog;

BOOL msglog_is_pos_in_range(msglog* in_msglog, int in_pos);

PROCESS_ID msglog_get_sender_pid(msglog* in_msglog, int in_pos);
PROCESS_ID msglog_get_destination_pid(msglog* in_msglog, int in_pos);
MESSAGE_TYPE msglog_get_msg_type(msglog* in_msglog, int in_pos);
SYSTIME msglog_get_transaction_time(msglog* in_msglog, int in_pos);
char* msglog_get_msgtext(msglog* in_msglog, int in_pos);

int msglog_get_nextpos(int in_currpos);
int msglog_save_transaction(msglog* in_msglog, envl* in_envl);
int msglog_print(msglog* in_msglog);

#endif /* MSGLOG_H */
