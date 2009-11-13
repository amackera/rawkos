#include "envl_deque.h"

envl* envl_dequeue_head(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL; 
    }

    if(is_envl_deque_empty(in_deque)) {
        return NULL; // dequeueing from an empty deque returns a NULL
    }

    // only 1 element on queue
    if(in_deque->head == in_deque->tail) {
        envl* curr = in_deque->head;
        curr->next = NULL;
        curr->prev = NULL;
        in_deque->head = NULL;
        in_deque->tail = NULL;
        return curr;
    }

    // >1 element on queue
    envl* curr = in_deque->head;
    curr->next->prev = NULL;
    in_deque->head = curr->next;
    curr->next = NULL;
    return curr;
}

envl* envl_dequeue_tail(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL; 
    }

    if(is_envl_deque_empty(in_deque)) {
        return NULL; // dequeueing from an empty deque returns a NULL
    }

    // only 1 element on queue
    if(in_deque->head == in_deque->tail) {
        envl* curr = in_deque->tail;
        curr->next = NULL;
        curr->prev = NULL;
        in_deque->head = NULL;
        in_deque->tail = NULL;
        return curr; // element now dequeued
    }
    // >1 element on queue
    envl* curr = in_deque->tail;
    curr->prev->next = NULL;
    in_deque->tail = curr->prev;
    curr->prev = NULL;
    return curr;
}

int envl_enque_head(envl* in_envl, envl_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }
    if(in_envl == NULL) {
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
        in_deque->head = in_envl;
        in_deque->tail = in_envl;
        in_envl->next = NULL;
        in_envl->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to same envl (only one element in deque)
    if(in_deque->head == in_deque->tail) {
        if(in_deque->tail == NULL) {
            return RTX_ERROR;
        }
        
        envl* curr = in_deque->head;
        
        // Element should not be pointing to anything, whether forward or back
        if(curr->next != NULL || curr->prev != NULL) {
            // me: likely an error
        }
        
        in_envl->next = curr;
        curr->prev = in_envl;
        in_deque->head = in_envl;
        in_envl->prev = NULL;
        curr->next = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to different envls (>=2 elements on deque)
    if(in_deque->head != in_deque->tail) {
        envl* curr = in_deque->head;
        curr->prev = in_envl;
        in_envl->next = curr;
        in_deque->head = in_envl;
        in_envl->prev = NULL;
        return RTX_SUCCESS;
    } 

    return RTX_ERROR; // shouldn't reach here
}

int envl_enque_tail(envl* in_envl, envl_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }
    if(in_envl == NULL) {
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
        in_deque->head = in_envl;
        in_deque->tail = in_envl;
        in_envl->next = NULL;
        in_envl->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to same envl (only one element in deque)
    if(in_deque->head == in_deque->tail) {
        if(in_deque->tail == NULL) {
            return RTX_ERROR;
        }

        envl* curr = in_deque->tail;

        // Element should not be pointing to anything, whether forward or back
        if(curr->next != NULL || curr->prev != NULL) {
            // me: likely an error
        }

        curr->next = in_envl;
        in_envl->prev = curr;
        in_deque->tail = in_envl;
        in_envl->next = NULL;
        curr->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to different envl (>=2 elements on deque)
    if(in_deque->head != in_deque->tail) {
        envl* curr = in_deque->tail;
        curr->next = in_envl;
        in_envl->prev = curr;
        in_deque->tail = in_envl;
        in_envl->next = NULL;
        return RTX_SUCCESS;
    }

    return RTX_ERROR; // shouldn't reach here
}

envl* envl_get_deque_head(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL;
    }
    return in_deque->head;
}

envl* envl_get_deque_tail(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL;
    }
    return in_deque->tail;
}

// Present deque as a queue
int envl_enque(envl* in_envl, envl_deque* in_deque) {
    return envl_enque_tail(in_envl, in_deque);
}

envl* envl_dequeue(envl_deque* in_deque) {
    return envl_dequeue_head(in_deque);
}

int is_envl_deque_empty(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }

    return in_deque->head == NULL && in_deque->tail == NULL ? TRUE : FALSE;
}

envl* envl_dequeue_by_sender_pid(envl_deque* in_deque, PROCESS_ID in_pid) {
    if(in_deque == NULL || !is_valid_pid(in_pid)) {
        return NULL;
    }

    envl* env = in_deque->head;
    while(env != NULL) {
        if(env->sender_pid == in_pid) {
            return env;
        }
        else {
            env = env->next;
        }
    }
    return NULL;
}

int envl_enque_by_delivery_time(envl_deque* in_deque, envl* in_envl) {
    // assume in_deque already sorted by delivery time
    // this function should only be called by delayed_send

    if(in_deque == NULL || in_envl == NULL) {
        return RTX_ERROR;
    }

    // if both are NULL, xnor both are not NULL
    if((in_deque->head == NULL && in_deque->tail == NULL) ==
       (in_deque->head != NULL && in_deque->tail != NULL)) {
        return RTX_ERROR;
    }

    // H and T point to nothing (empty deque)
    if(in_deque->head == NULL && in_deque->tail == NULL) {
        in_deque->head = in_envl;
        in_deque->tail = in_envl;
        in_envl->next = NULL;
        in_envl->prev = NULL;
        return RTX_SUCCESS;
    }

    // H and T point to same envl (only one element in deque)
    if(in_deque->head == in_deque->tail) {
        if(in_deque->tail == NULL) {
            return RTX_ERROR;
        }
        
        envl* curr = in_deque->tail;
        
        // Element should not be pointing to anything, whether forward or back
        if(curr->next != NULL || curr->prev != NULL) {
            // me: likely an error
        }
        
        if(in_envl->delivery_time < curr->delivery_time) {
            // enque in_envl before curr
            in_envl->next = curr;
            curr->prev = in_envl;
            in_deque->head = in_envl;
            in_envl->prev = NULL;
            curr->next = NULL;
        }
        else {
            // enque in_envl after curr
            curr->next = in_envl;
            in_envl->prev = curr;
            in_deque->tail = in_envl;
            curr->prev = NULL;
            in_envl->next = NULL;
        }
        return RTX_SUCCESS;
    }

    // H and T point to different envl (>=2 elements on deque)
    if(in_deque->head != in_deque->tail) {
        if(in_envl->delivery_time < in_deque->head->delivery_time) {
            in_envl->next = in_deque->head;
            in_deque->head->prev = in_envl;
            in_deque->head = in_envl;
            in_envl->prev = NULL;
            return RTX_SUCCESS;
        }

        if(in_envl->delivery_time > in_deque->tail->delivery_time) {
            in_deque->tail->next = in_envl;
            in_envl->prev = in_deque->tail;
            in_deque->tail = in_envl;
            in_envl->next = NULL;
            return RTX_SUCCESS;
        }

        // in_envl goes somewhere in between (at least 3 elements)

        // starting at head and going in ascending order
        envl* curr = in_deque->head->next;
        while(curr != in_deque->tail) {
            if(in_envl->delivery_time < curr->delivery_time) {
                in_envl->next = curr;
                in_envl->prev = curr->prev;
                curr->prev->next = in_envl;
                curr->prev = in_envl;
                return RTX_SUCCESS;
            }
            curr = curr->next;
        }
    }

    return RTX_ERROR; // shouldn't reach here
}

envl* envl_peek_head(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return NULL;
    }

    return in_deque->head;
}

int envl_print_deque(envl_deque* in_deque) {
    if(in_deque == NULL) {
        return RTX_ERROR;
    }

    envl* curr = in_deque->head;
    if(curr == NULL) {
        return RTX_SUCCESS;
    }

    while(curr != in_deque->tail) {
        envl_print(curr);
        curr = curr->next;
    }
    envl_print(curr); // print last element
    return RTX_SUCCESS;
}
