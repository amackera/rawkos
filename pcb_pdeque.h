#ifndef PCB_PDEQUE_H
#define PCB_PDEQUE_H

#include "pcb_deque.h"

int is_pcb_pdeque_empty(pcb_deque* in_pcb_pdeque);
//int is_pcb_pdeque_empty(pcb_deque in_pcb_pdeque[]); // equivalent to above in our case
pcb* pcb_deque_highest(pcb_deque* in_pcb_pdeque);
pcb* pcb_peek_highest(pcb_deque* in_pcb_pdeque);

#endif /* PCB_PDEQUE_H */
