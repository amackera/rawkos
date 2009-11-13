#ifndef INIT_H
#define INIT_H

#include "globalvar.h"
#include "pcb_pdeque.h"
#include "envl_deque.h"
#include "scheduler.h"
#include "nullproc.h"
#include "swi.h"
#include "testproc.h"
#include "kcd.h"
#include "crt.h"
#include "timer.h"
#include "uart.h"
#include "spc.h"
#include "wall_clock.h"
#include "func.h"
#ifdef _RTX_EVAL_
#include "rtx.h"
#endif

typedef struct {
    int pid;
    PRIORITY_LEVEL priority;
    int stack_size; // to be adjusted when allocating stacks
    void* code_pointer;
} pcb_init_entry;

pcb_init_entry pcb_init_table[DEFAULT_PCBCNT];

int init();

#endif /* INIT_H */
