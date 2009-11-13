#ifndef ENVL_DEQUE_H
#define ENVL_DEQUE_H

#include "envl.h"

typedef struct {
    envl* head;
    envl* tail;
} envl_deque;

envl* envl_get_deque_head(envl_deque* in_deque);
envl* envl_get_deque_tail(envl_deque* in_deque);

envl* envl_dequeue_tail(envl_deque* in_deque);
envl* envl_dequeue_head(envl_deque* in_deque);
envl* envl_dequeue(envl_deque* in_deque); // note: NULL deques return NULL envelopes

int envl_enque_tail(envl* in_envl, envl_deque* in_deque);
int envl_enque_head(envl* in_envl, envl_deque* in_deque);
int envl_enque(envl* in_envl, envl_deque* in_deque);

int is_envl_deque_empty(envl_deque* in_deque);
envl* envl_dequeue_by_sender_pid(envl_deque* in_deque, PROCESS_ID in_pid);
int envl_enque_by_delivery_time(envl_deque* in_deque, envl* in_envl);

envl* envl_peek_head(envl_deque* in_deque);

int envl_print_deque(envl_deque* in_deque);

#endif /* ENVL_DEQUE_H */
