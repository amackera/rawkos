#ifndef WALL_CLOCK_H
#define WALL_CLOCK_H

#include "globalvar.h"
#include "ipc.h"
#include "kcd.h"
#include "mem.h"
#include "timesrv.h"
#include "func.h"

int parse_time(char* in_time);
void wall_clock_display_process();

#endif /* WALL_CLOCK_H */
