#ifndef PROCMAN_H
#define PROCMAN_H

// process management

#include "globalvar.h"
#include "pcb_pdeque.h"
#include "func.h"
#include "scheduler.h"
#include "outs.h"

int release_processor();
int release_processor_handler();
void process_switch(pcb* in_next_proc);
void context_switch(pcb* in_next_proc);

#endif /* PROCMAN_H */
