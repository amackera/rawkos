#ifndef IPC_H
#define IPC_H

#include "globalvar.h"
#include "func.h"
#include "envl_deque.h"
#include "procman.h"

int send_message(int process_ID, void* MessageEnvelope);
int send_message_handler(int process_ID, void* MessageEnvelope);
void* receive_message(int* sender_ID);
void* receive_message_handler(int* sender_ID);

#endif /* IPC_H */
