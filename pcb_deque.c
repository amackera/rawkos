#include "pcb_deque.h"

pcb* pcb_dequeue_head(pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL; 
    }
    if(in_deque->head == NULL) {
        return NULL; // error occured
    }

    // only 1 element on queue
    if(in_deque->head == in_deque->tail) {
        pcb* curr = in_deque->head;
        curr->next = NULL;
        curr->prev = NULL;
        in_deque->head = NULL;
        in_deque->tail = NULL;
        return curr;
    }

    // >1 element on queue
    pcb* curr = in_deque->head;
    curr->next->prev = NULL;
    in_deque->head = curr->next;
    curr->next = NULL;
    return curr;
}

pcb* pcb_dequeue_tail(pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL; 
    }
    if(in_deque->tail == NULL) {
        return NULL; // error occured
    }

    // only 1 element on queue
    if(in_deque->head == in_deque->tail) {
        pcb* curr = in_deque->tail;
        curr->next = NULL;
        curr->prev = NULL;
        in_deque->head = NULL;
        in_deque->tail = NULL;
        return curr; // element now dequeued
    }

    // >1 element on queue
    pcb* curr = in_deque->tail;
    curr->prev->next = NULL;
    in_deque->tail = curr->prev;
    curr->prev = NULL;
    return curr;
}

int pcb_enque_head(pcb* in_pcb, pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    // if both are NULL, xnor both are not NULL
    if((in_deque->head == NULL && in_deque->tail == NULL) ==
       (in_deque->head != NULL && in_deque->tail != NULL)) {
        return RTX_ERROR;
    }

    // either both NULL, xor neither NULL

    // H and T point to nothing (empty deque)
    if(in_deque->head == NULL && in_deque->tail == NULL) {
        in_deque->head = in_pcb;
        in_deque->tail = in_pcb;
        in_pcb->next = NULL;
        in_pcb->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to same PCB (only one element in deque)
    if(in_deque->head == in_deque->tail) {
        if(in_deque->tail == NULL) {
            return RTX_ERROR;
        }

        pcb* curr = in_deque->head;
        
        // Element should not be pointing to anything, whether forward or back
        if(curr->next != NULL || curr->prev != NULL) {
          // me: likely an error
        }
        
        in_pcb->next = curr;
        curr->prev = in_pcb;
        in_deque->head = in_pcb;
        in_pcb->prev = NULL;
        curr->next = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to different PCBs (>=2 elements on deque)
    if(in_deque->head != in_deque->tail) {
        pcb* curr = in_deque->head;
        curr->prev = in_pcb;
        in_pcb->next = curr;
        in_deque->head = in_pcb;
        in_pcb->prev = NULL;
        return RTX_SUCCESS;
    }

    return RTX_ERROR; // shouldn't reach here
}

int pcb_enque_tail(pcb* in_pcb, pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    // if both are NULL, xnor both are not NULL
    if((in_deque->head == NULL && in_deque->tail == NULL) ==
       (in_deque->head != NULL && in_deque->tail != NULL)) {
        return RTX_ERROR;
    }

    // either both NULL, xor neither NULL

    // H and T point to nothing (empty deque)
    if(in_deque->head == NULL && in_deque->tail == NULL) {
        in_deque->head = in_pcb;
        in_deque->tail = in_pcb;
        in_pcb->next = NULL;
        in_pcb->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to same PCB (only one element in deque)
    if(in_deque->head == in_deque->tail) {
        if(in_deque->tail == NULL) {
            return RTX_ERROR;
        }

        pcb* curr = in_deque->tail;

        // Element should not be pointing to anything, whether forward or back
        if(curr->next != NULL || curr->prev != NULL) {
            // me: likely an error
        }

        curr->next = in_pcb;
        in_pcb->prev = curr;
        in_deque->tail = in_pcb;
        in_pcb->next = NULL;
        curr->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to different PCBs (>=2 elements on deque)
    if(in_deque->head != in_deque->tail) {
        pcb* curr = in_deque->tail;
        curr->next = in_pcb;
        in_pcb->prev = curr;
        in_deque->tail = in_pcb;
        in_pcb->next = NULL;
        return RTX_SUCCESS;
    }

    return RTX_ERROR; // shouldn't reach here
}

// Present deque as a queue
int pcb_enque(pcb* in_pcb, pcb_deque* in_deque) {
    return pcb_enque_tail(in_pcb, in_deque);
}

pcb* pcb_dequeue(pcb_deque* in_deque) {
    return pcb_dequeue_head(in_deque);
}

pcb* pcb_dequeue_by_pid(pcb_deque* in_deque, PROCESS_ID in_pid) {
    if(in_deque == NULL || !is_valid_pid(in_pid)) {
        return NULL;
    }

    if(is_pcb_deque_empty(in_deque)) {
        return NULL; // unable to find pcb with in_pid
    }

    // at least 1 element
    pcb* curr = in_deque->head;
    if(curr == in_deque->tail) { // 1 element in list
        if(curr->pid == in_pid) {
            in_deque->head = NULL;
            in_deque->tail = NULL;
            return curr;
        }
        else {
            return NULL;
        }
    }

    // 1+ elements are in list
    while(curr != in_deque->tail) {
        if(curr->pid == in_pid) {
            if(curr == in_deque->head) {
                in_deque->head = curr->next;
                curr->next->prev = NULL;
                curr->next = NULL;
                curr->prev = NULL;
                return curr;
            }
            else {
                curr->next->prev = curr->prev;
                curr->prev->next = curr->next;
                curr->prev = NULL;
                curr->next = NULL;
                return curr;
            }
        }
        curr = curr->next;
    }
    if(curr->pid == in_pid) { // element at in_deque->tail
        in_deque->tail = curr->prev;
        curr->prev->next = NULL;
        curr->prev = NULL;
        curr->next = NULL;
        return curr;
    }
    return NULL; // couldn't find the pcb with in_pid on the deque
}

int is_pcb_deque_empty(pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }

    return in_deque->head == NULL && in_deque->tail == NULL ? TRUE : FALSE;
}

pcb* pcb_peek_head(pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL;
    }

    return in_deque->head;
}

int pcb_print_pcb_deque(pcb_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }

    pcb* curr = in_deque->head;
    if(curr == NULL) {
        return RTX_SUCCESS;
    }

    while(curr != in_deque->tail) {
        pcb_print(curr);
        curr = curr->next;
    }
    pcb_print(curr); // print last element
    return RTX_SUCCESS;
}

int is_pcb_on_deque(pcb_deque* in_deque, pcb* in_pcb) {
    if(in_deque == NULL || in_pcb == NULL) {
        return RTX_ERROR;
    }

    pcb* curr = in_deque->head;
    if(curr == NULL) { // empty deque
        return FALSE;
    }

    while(curr != in_deque->tail) {
        if(curr->pid == in_pcb->pid) {
            return TRUE;
        }
        curr = curr->next;
    }
    if(curr->pid == in_pcb->pid) {
        return TRUE;
    }
    return FALSE;
}
