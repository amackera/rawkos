#ifndef PCB_DEQUE_H
#define PCB_DEQUE_H

#include "pcb.h"

typedef struct {
    pcb* head;
    pcb* tail;
} pcb_deque;

int is_pcb_deque_empty(pcb_deque* in_deque);
int is_pcb_on_deque(pcb_deque* in_deque, pcb* in_pcb);

pcb* pcb_dequeue_tail(pcb_deque* in_deque);
pcb* pcb_dequeue_head(pcb_deque* in_deque);
pcb* pcb_dequeue(pcb_deque* in_deque);
pcb* pcb_dequeue_by_pid(pcb_deque* in_deque, PROCESS_ID in_pid);

int pcb_enque_tail(pcb* in_pcb, pcb_deque* in_deque);
int pcb_enque_head(pcb* in_pcb, pcb_deque* in_deque);
int pcb_enque(pcb* in_pcb, pcb_deque* in_deque);

pcb* pcb_peek_head(pcb_deque* in_deque);
int pcb_print_pcb_deque(pcb_deque* in_deque);

#endif /* PCB_DEQUE_H */
