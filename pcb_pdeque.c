#include "pcb_pdeque.h"

int is_pcb_pdeque_empty(pcb_deque* in_pcb_pdeque) {
    if(in_pcb_pdeque == NULL) {
        return RTX_ERROR;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(!is_pcb_deque_empty(&in_pcb_pdeque[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

pcb* pcb_deque_highest(pcb_deque* in_pcb_pdeque) {
    if(in_pcb_pdeque == NULL) {
        // remember, dequing from a NULL queue returns NULL; not an error, per-se
        return NULL;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(!is_pcb_deque_empty(&in_pcb_pdeque[i])) {
            return pcb_dequeue(&in_pcb_pdeque[i]);
        }
    }
    return NULL;    
}

pcb* pcb_peek_highest(pcb_deque* in_pcb_pdeque) {
    if(in_pcb_pdeque == NULL) {
        return NULL;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(!is_pcb_deque_empty(&in_pcb_pdeque[i])) {
            return pcb_peek_head(&in_pcb_pdeque[i]);
        }
    }
    return NULL; // nothing found
}
