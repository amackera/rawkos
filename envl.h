#ifndef ENVL_H
#define ENVL_H

#include "const.h"
#include "outs.h"

#define ENVL_RESERVED_SIZE 32
#define ENVL_MSGTEXT_SIZE 64

typedef struct envl {
    // message header
    // 32 bytes
    struct envl* next; // used to build message deque
    struct envl* prev;
    PROCESS_ID sender_pid;
    PROCESS_ID destination_pid;
    MESSAGE_TYPE msg_type;
    SYSTIME sent_time;
    SYSTIME delivery_time; // indicates when this message should be received (for delayed_send())

    /*
"Test processes will use an offset of 64-bytes from the pointer (to a
message envelope) and use that area (within the message text) for inter
test process communication. So, none of your primitives should modify
this message text area while handling a message envelope
(i.e. deallocate, allocate, delay, etc.). This area, within the message
text, is only used by the test processes while they "own" an
envelope. Of course, your own processes can do anything they want with
this area when they "own" an envelope.
"
    */
    // message text
    // 96 bytes
    char reserved[ENVL_RESERVED_SIZE];
    char msgtext[ENVL_MSGTEXT_SIZE];
} envl;

int envl_clear(envl* in_envl);

int envl_set_sender(envl* env, PROCESS_ID sender_pid);
int envl_set_destination(envl* env, PROCESS_ID destination_pid);
int envl_set_sent_time(envl* in_envl, SYSTIME in_stime);
int envl_set_delivery_time(envl* in_envl, SYSTIME in_dtime);
int envl_set_message_type(envl* env, MESSAGE_TYPE msg_type);
//int envl_set_msgtext(envl* in_envl, char* in_msgtext); // moved to func.c

PROCESS_ID envl_get_sender_id(envl* in_envl);
MESSAGE_TYPE envl_get_message_type(envl* in_envl);
PROCESS_ID envl_get_destination_pid(envl* in_envl);
SYSTIME envl_get_sent_time(envl* in_envl);
SYSTIME envl_get_delivery_time(envl* in_envl);
char* envl_get_msgtext(envl* in_envl);

int envl_print(envl* in_envl);

#endif /* ENVL_H */
