#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "globalvar.h"
#include "pcb_pdeque.h"
#include "func.h"
#include "procman.h"
#include "outs.h"

void check_schedule();
pcb* scheduler();

#endif /* SCHEDULER_H */
