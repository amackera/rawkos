#include "pcb.h"

BOOL pcb_is_i_process(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    return in_pcb->i_process;
}

// me: many of these set functions should only be called in init, or by system processes

int pcb_set_state(pcb* in_pcb, PROCESS_STATE in_state) {
    if(!is_valid_state(in_state)) {
        return RTX_ERROR;
    }

    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    in_pcb->state = in_state;
    return RTX_SUCCESS;
}

int pcb_set_priority(pcb* in_pcb, PRIORITY_LEVEL in_plvl) {
    if(in_pcb == NULL || !is_valid_priority(in_plvl)) {
        return RTX_ERROR;
    }
    in_pcb->priority = in_plvl;
    return RTX_SUCCESS;
}

int pcb_set_stack_pointer(pcb* in_pcb, void* in_sp) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }
    in_pcb->stack_pointer = in_sp;
    return RTX_SUCCESS;
}

int pcb_set_stack_start_addr(pcb* in_pcb, void* in_sp) {
    if(in_pcb == NULL) {
        return NULL;
    }
    in_pcb->stack_start_addr = in_sp;
    return RTX_SUCCESS;
}

int pcb_set_stack_end_addr(pcb* in_pcb, void* in_sp) {
    if(in_pcb == NULL) {
        return NULL;
    }
    in_pcb->stack_end_addr = in_sp;
    return RTX_SUCCESS;
}

int pcb_set_code_pointer(pcb* in_pcb, void* in_sp) {
    if(in_pcb == NULL) {
        return NULL;
    }
    in_pcb->code_pointer = in_sp;
    return RTX_SUCCESS;
}













PROCESS_STATE pcb_get_state(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }
    return in_pcb->state;
}

PROCESS_ID pcb_get_pid(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }
    return in_pcb->pid;
}

char* pcb_get_name(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return "NULL";
    }
    return in_pcb->name;
}

PRIORITY_LEVEL pcb_get_priority(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }
    return in_pcb->priority;
}

envl_deque* pcb_get_message_queue(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return NULL;
    }
    return &(in_pcb->messages);
}

void* pcb_get_code_pointer(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return NULL;
    }
    return in_pcb->code_pointer;
}

void* pcb_get_stack_pointer(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return NULL;
    }
    return in_pcb->stack_pointer;
}

void* pcb_get_stack_start_addr(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return NULL;
    }
    return in_pcb->stack_start_addr;
}

void* pcb_get_stack_end_addr(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return NULL;
    }
    return in_pcb->stack_end_addr;
}

/*
int enque_envl(pcb* in_pcb, envl* in_msgenv) {
    if(in_pcb == NULL || in_msgenv == NULL) {
        return RTX_ERROR;
    }

    envl_enque(in_msgenv, pcb_get_message_queue(in_pcb));
    return RTX_SUCCESS;
}

envl* dequeue_envl(pcb* in_pcb) {
    // should return NULL if nothing to dequeue
    return envl_dequeue(pcb_get_message_queue(in_pcb));
}
*/

int pcb_print(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    /*
    rtx_dbug_outs("in_pcb: ");
    rtx_dbug_out_int16((UINT32)in_pcb);
    rtx_dbug_outs("\n\r");
    rtx_dbug_outs("next: ");
    rtx_dbug_out_int16((UINT32)in_pcb->next);
    rtx_dbug_outs("\n\r");
    rtx_dbug_outs("prev: ");
    rtx_dbug_out_int16((UINT32)in_pcb->prev);
    rtx_dbug_outs("\n\r");
    rtx_dbug_outs("pid: ");
    rtx_dbug_out_int16(in_pcb->pid);
    rtx_dbug_outs("\n\r");
    */
    rtx_dbug_outs("name: ");
    rtx_dbug_outs(in_pcb->name);
    //rtx_dbug_outs("\n\r");
    rtx_dbug_outs("; ");
    rtx_dbug_outs("priority: ");
    rtx_dbug_out_int16(in_pcb->priority);
    //rtx_dbug_outs("\n\r");
    rtx_dbug_outs("; ");
    rtx_dbug_outs("state: ");
    rtx_dbug_outs(get_process_state_name(in_pcb->state));
    rtx_dbug_outs("\n\r");



    return RTX_SUCCESS;
}
