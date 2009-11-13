#ifndef TIMESRV_H
#define TIMESRV_H

#include "globalvar.h"
#include "scheduler.h"
#include "func.h"

int delayed_send(int process_ID, void* MessageEnvelope, int delay);
int delayed_send_handler(int process_ID, void* MessageEnvelope, int delay);

#endif /* TIMESRV_H */
