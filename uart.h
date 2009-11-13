#ifndef UART_H
#define UART_H

#include "globalvar.h"
#include "ipc.h"
#include "func.h"
#include "pcb.h"
#include "procman.h"
#include "mem.h"
#include "outs.h"

void c_uart_handler();
void uart_i_process();

#endif /* UART_H */
