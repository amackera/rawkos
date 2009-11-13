#ifndef PROCPRI_H
#define PROCPRI_H

#include "pcb.h"
#include "func.h"
#include "scheduler.h"

int set_process_priority(int process_ID, int priority);
int set_process_priority_handler(int process_ID, int priority);
int get_process_priority (int process_ID);
int get_process_priority_handler(int process_ID);

#endif /* PROCPRI_H */
