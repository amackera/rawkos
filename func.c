#include "func.h"

// Traverse through pcb table/list and return process with given pid
// Return NULL if nothing found
// Returns pcb with given in_pid if found. Dequeues the pcb if it is on any queue
pcb* get_pcb(PROCESS_ID in_pid) {
    int i = 0;
    pcb* found_pcb = NULL;
    for(; i<pcbcnt; i++) {
        if(pcb_table[i] != NULL) {
            if(pcb_table[i]->pid == in_pid) {
                found_pcb = pcb_table[i];
                break;
            }
        }
        else {
            // me: shouldn't be any null pointers in pcb list
        }
    }
    if(found_pcb == NULL) {
        // couldn't find the process
        return NULL;
    }

    PROCESS_STATE curr_state = pcb_get_state(found_pcb);
    BOOLEAN i_proc = pcb_is_i_process(found_pcb);
    if(i_proc == TRUE) {
        // i-process is not on any queue (or shouldn't be)
        return found_pcb;
    }

    if(curr_state == READY) {
        found_pcb = dequeue_from_ready_queue(in_pid);
    }
    else if(curr_state == BLOCKED_ON_MEMORY) {
        found_pcb = dequeue_from_blocked_on_memory_queue(in_pid);
    }
    else if(curr_state == BLOCKED_ON_MESSAGE) {
        found_pcb = dequeue_from_blocked_on_receive_queue(in_pid);
    }
    else if(curr_state == RUNNING) {
        found_pcb = current_process;
    }
    else {
        // me: error
        return NULL;
    }
    if(found_pcb == NULL) {
        return NULL; // couldn't find in the queue given by the pcb's current state
    }
    return found_pcb;
}

pcb* peek_pcb(PROCESS_ID in_pid) {
    int i = 0;
    for(; i<pcbcnt; i++) {
        if(pcb_table[i] != NULL) {
            if(pcb_table[i]->pid == in_pid) {
                return pcb_table[i];
            }
        }
        else {
            // me: shouldn't be any null pointers in pcb list
        }
    }
    // couldn't find the process
    return NULL;
}

envl* get_envl_by_sender_pid(pcb* in_pcb, PROCESS_ID* in_pid) {
    if(in_pcb == NULL) {
        return NULL;
    }

    if(in_pid == NULL) {
        return envl_dequeue(pcb_get_message_queue(in_pcb)); // return first envelope
    }
    else {
        return envl_dequeue_by_sender_pid(pcb_get_message_queue(in_pcb), *in_pid); // deques first envl with given in_pid
    }
}








envl* dequeue_from_free_mem_queue() {
    --free_mem_queue_cnt;
    return envl_dequeue(free_mem_queue); // will return NULL if nothing available
}

int enque_onto_free_mem_queue(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_SUCCESS; // successfully freed nothing
    }

    envl_clear(in_envl);
    envl_enque(in_envl, free_mem_queue);
    ++free_mem_queue_cnt;
    return RTX_SUCCESS;
}

envl* dequeue_from_blocked_mem_queue() {
    --blocked_mem_queue_cnt;
    return envl_dequeue(blocked_mem_queue); // will return NULL if nothing available
}

int enque_onto_blocked_mem_queue(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_SUCCESS; // successfully freed nothing
    }

    envl_enque(in_envl, blocked_mem_queue);
    ++blocked_mem_queue_cnt;
    return RTX_SUCCESS;
}

envl* dequeue_from_delay_send_queue() {
    --delay_send_queue_cnt;
    return envl_dequeue(delay_send_queue); // will return NULL if nothing available
}

int enque_onto_delay_send_queue(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_SUCCESS; // successfully freed nothing
    }

    envl_enque(in_envl, delay_send_queue);
    ++delay_send_queue_cnt;
    return RTX_SUCCESS;
}

int enque_onto_delay_send_queue_by_delivery_time(envl* in_envl) {
    if(in_envl == NULL) {
        return RTX_SUCCESS; // successfully enqued nothing
    }

    envl_enque_by_delivery_time(delay_send_queue, in_envl);
    ++delay_send_queue_cnt;
    return RTX_SUCCESS;
}















int clear_buffer(char* in_buf, int count) {
    if(count <= 0) {
        return RTX_ERROR;
    }

    if(in_buf == NULL) {
        return RTX_SUCCESS;
    }

    int i;
    for(i=0; i<count; i++) {
        in_buf[i] = '\0';
    }
    return RTX_SUCCESS;
}

int copy_buffer(char* in_src_buf, char* in_dest_buf, int in_num) {
    // note: sizeof(in_dest_buf) >= sizeof(in_src_buf)
    // also, in_src_buf should be null terminated
    // also, in_dest_buf will NOT be null_terminated if sizeof(in_src_buf) >= in_num
    int cnt = 0; // current index
    // copy upto in_num
    while(in_src_buf[cnt] != '\0') {
        if(cnt >= in_num) {
            return RTX_SUCCESS;
        }
        in_dest_buf[cnt] = in_src_buf[cnt];
        ++cnt;
    }

    // sizeof(in_src_buf) < sizeof(in_dest_buf)
    // Pad remainder of in_dest_buf with null chars
    while(cnt < in_num) {
        in_dest_buf[cnt] = '\0';
        ++cnt;
    }
    return RTX_SUCCESS;
}

/*
int set_process_state(pcb* in_proc, PROCESS_STATE in_state) {
    return pcb_set_state(in_proc, in_state);
}
*/

int enque_onto_ready_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl) {
    if(in_pcb == NULL || !is_valid_priority(in_plvl)) {
        return RTX_ERROR;
    }
    pcb_set_state(in_pcb, READY);
    pcb_set_priority(in_pcb, in_plvl);

    int res = 0;
    res = pcb_enque(in_pcb, &ready_queue[in_plvl]);
#ifdef DEBUG
    if(res == RTX_ERROR) {
        rtx_dbug_outs("[ENQUE_ONTO_READY_QUEUE][ERROR] Failed to enque");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs("\n\r");
    }
    else {
        rtx_dbug_outs("[ENQUE_ONTO_READY_QUEUE] Successfully enqued ");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs(" at plvl ");
        rtx_dbug_out_int10(pcb_get_priority(in_pcb));
        rtx_dbug_outs(" with state ");
        rtx_dbug_outs(get_process_state_name(pcb_get_state(in_pcb)));
        rtx_dbug_outs("\n\r");
        /*
        // very slow if used
        // I used this to detect if something was actually enqued onto this queue
        rtx_dbug_outs("[ENQUE_ONTO_READY_QUEUE] Printing ready_queue\n\r");
        print_ready_queue();
        */
    }
#endif
    return RTX_SUCCESS;
}

int enque_onto_blocked_on_receive_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl) {
    if(in_pcb == NULL || !is_valid_priority(in_plvl)) {
        return RTX_ERROR;
    }
    pcb_set_state(in_pcb, BLOCKED_ON_MESSAGE);
    pcb_set_priority(in_pcb, in_plvl);

    int res = 0;
    res = pcb_enque(in_pcb, &blocked_on_receive_queue[in_plvl]);
#ifdef DEBUG
    if(res == RTX_ERROR) {
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_RECEIVE_QUEUE][ERROR] Failed to enque");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs("\n\r");
    }
    else {
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_RECEIVE_QUEUE] Successfully enqued ");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs(" at plvl ");
        rtx_dbug_out_int10(pcb_get_priority(in_pcb));
        rtx_dbug_outs(" with state ");
        rtx_dbug_outs(get_process_state_name(pcb_get_state(in_pcb)));
        rtx_dbug_outs("\n\r");

        /*
        // very slow if used
        // I used this to detect if something was actually enqued onto this queue
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_RECEIVE_QUEUE] Printing blocked_on_receive_queue\n\r");
        print_blocked_on_receive_queue();
        */
    }
#endif
    return RTX_SUCCESS;
}

int enque_onto_blocked_on_memory_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl) {
    if(in_pcb == NULL || !is_valid_priority(in_plvl)) {
        return RTX_ERROR;
    }
    pcb_set_state(in_pcb, BLOCKED_ON_MEMORY);
    pcb_set_priority(in_pcb, in_plvl);

    int res = 0;
    res = pcb_enque(in_pcb, &blocked_on_memory_queue[in_plvl]);
#ifdef DEBUG
    if(res == RTX_ERROR) {
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_MEMORY_QUEUE][ERROR] Failed to enque");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs("\n\r");
    }
    else {
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_MEMORY_QUEUE] Successfully enqued ");
        rtx_dbug_outs(pcb_get_name(in_pcb));
        rtx_dbug_outs(" at plvl ");
        rtx_dbug_out_int10(pcb_get_priority(in_pcb));
        rtx_dbug_outs(" with state ");
        rtx_dbug_outs(get_process_state_name(pcb_get_state(in_pcb)));
        rtx_dbug_outs("\n\r");

        /*
        // very slow if used
        // I used this to detect if something was actually enqued onto this queue
        rtx_dbug_outs("[ENQUE_ONTO_BLOCKED_ON_MEMORY_QUEUE] Printing blocked_on_memory_queue\n\r");
        print_blocked_on_memory_queue();
        */
    }
#endif
    return RTX_SUCCESS;
}

pcb* dequeue_from_ready_queue(PROCESS_ID in_pid) {
    int i = 0;
    pcb* found_pcb = NULL;
    int res = 0;
    for(; i < NUM_PRIORITY; i++) {
        res = is_pcb_deque_empty(&ready_queue[i]);
        if(res == FALSE) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_READY_QUEUE] Searching for ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" on ready_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
            found_pcb = pcb_dequeue_by_pid(&ready_queue[i], in_pid);
            if(found_pcb != NULL) {
                /*
#ifdef DEBUG
                rtx_dbug_outs("found_pcb = ");
                rtx_dbug_out_int16(found_pcb);
                rtx_dbug_outs("\n\r");
#endif
                */
                return found_pcb;
            }
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_READY_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" not found on ready_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
        }
        if(res == RTX_ERROR) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_READY_QUEUE][ERROR] ready_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("] is empty\n\r");
#endif
        }

        /*
        // problematic code
        if(!is_pcb_deque_empty(&ready_queue[i])) {
            found_pcb = pcb_dequeue_by_pid(&ready_queue[i], in_pid);
            if(found_pcb != NULL) {
                return found_pcb;
            }
        }
        */
    }
#ifdef DEBUG
    rtx_dbug_outs("[DEQUEUE_FROM_READY_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
    rtx_dbug_outs(" not found on ready_queue\n\r");
#endif
    return NULL;
}

pcb* dequeue_from_blocked_on_receive_queue(PROCESS_ID in_pid) {
    int i = 0;
    pcb* found_pcb = NULL;
    int res = 0;
    for(; i < NUM_PRIORITY; i++) {
        res = is_pcb_deque_empty(&blocked_on_receive_queue[i]);
        if(res == FALSE) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_RECEIVE_QUEUE] Searching for ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" on blocked_on_receive_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
            found_pcb = pcb_dequeue_by_pid(&blocked_on_receive_queue[i], in_pid);
            if(found_pcb != NULL) {
                /*                
#ifdef DEBUG
                rtx_dbug_outs("found_pcb = ");
                rtx_dbug_outs(pcb_get_name(found_pcb));
                rtx_dbug_outs("\n\r");
#endif
                */
                return found_pcb;
            }
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_RECEIVE_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" not found on blocked_on_receive_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
        }
        else if(res == RTX_ERROR) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_RECEIVE_QUEUE] blocked_on_receive_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("] is empty\n\r");
#endif
        }
        /*
        // problematic code
        if(!is_pcb_deque_empty(&blocked_on_receive_queue[i])) {
            found_pcb = pcb_dequeue_by_pid(&blocked_on_receive_queue[i], in_pid);

            if(found_pcb != NULL) {
                return found_pcb;
            }
        }
        */
    }
#ifdef DEBUG
    rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_RECEIVE_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
    rtx_dbug_outs(" not found on blocked_on_receive_queue\n\r");
#endif
    return NULL;
}

pcb* dequeue_from_blocked_on_memory_queue(PROCESS_ID in_pid) {
    int i = 0;
    pcb* found_pcb = NULL;
    int res = 0;
    for(; i < NUM_PRIORITY; i++) {
        res = is_pcb_deque_empty(&blocked_on_memory_queue[i]);
        if(res == FALSE) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_MEMORY_QUEUE] Searching for ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" on blocked_on_memory_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
            found_pcb = pcb_dequeue_by_pid(&blocked_on_memory_queue[i], in_pid);
            if(found_pcb != NULL) {
                /*
#ifdef DEBUG
                rtx_dbug_outs("found_pcb = ");
                rtx_dbug_out_int16(found_pcb);
                rtx_dbug_outs("\n\r");
#endif
                */
                return found_pcb;
            }
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_MEMORY_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
            rtx_dbug_outs(" not found on blocked_on_memory_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("]\n\r");
#endif
        }
        else if(res == RTX_ERROR) {
#ifdef DEBUG
            rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_MEMORY_QUEUE][ERROR] blocked_on_memory_queue[");
            rtx_dbug_out_int10(i);
            rtx_dbug_outs("] is empty\n\r");
#endif
        }
        /*
        // problematic code
        if(!is_pcb_deque_empty(&blocked_on_memory_queue[i])) {
            found_pcb = pcb_dequeue_by_pid(&blocked_on_memory_queue[i], in_pid);

            if(found_pcb != NULL) {
                return found_pcb;
            }
        }
        */
    }
#ifdef DEBUG
    rtx_dbug_outs("[DEQUEUE_FROM_BLOCKED_ON_MEMORY_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(peek_pcb(in_pid)));
    rtx_dbug_outs(" not found on blocked_on_memory_queue");
#endif
    return NULL;
}

int atomic(int in_state) {
    /*
      UINT32 mask;
      if(in_state == ON) {  // Disable Interrupts
      mask = SIM_IMR;
      mask |= 0xFFFFFFFF;    
      SIM_IMR = mask;    
      return RTX_SUCCESS;
      }
      else if(in_state == OFF) {  // Enable Interrupts
      mask = SIM_IMR;
      mask &= 0x0003cdff;
      SIM_IMR = mask;    
      return RTX_SUCCESS;
      }
      else {
      return RTX_ERROR;
      }
    */
    return RTX_SUCCESS;
}

// Converts from a char that is a number to a real number or else will return -1
int char_to_int(char in_num) { 
    switch((int)in_num){
    case((int)('0')): return 0;
    case((int)('1')): return 1;
    case((int)('2')): return 2;
    case((int)('3')): return 3;
    case((int)('4')): return 4;
    case((int)('5')): return 5;
    case((int)('6')): return 6;
    case((int)('7')): return 7;
    case((int)('8')): return 8;
    case((int)('9')): return 9;
    default: return RTX_ERROR;
    }
}

// Returns -1 as the integer if there are letters or special chars in the input param 
int atoi(char* in_string) {
    char * tmp_string = in_string;
    int tmp_int = 0;
    int result = 0;

    for(; *tmp_string != '\0'; tmp_string++) {
        result = char_to_int(*tmp_string);
        if (result != RTX_ERROR) { 
            tmp_int = (tmp_int * 10) + result;
        }
        else {
            return RTX_ERROR;
        }
    }
    return tmp_int;
}

char int_to_char(int in_num) {
    switch(in_num) {
    case(0): return '0';
    case(1): return '1';
    case(2): return '2';
    case(3): return '3';
    case(4): return '4';
    case(5): return '5';
    case(6): return '6';
    case(7): return '7';
    case(8): return '8';
    case(9): return '9';
    default: return RTX_ERROR;
    }
}

char* itoa(int value, char* str, int base) {
    // for our itoa, base = 10
    // str should be null terminated at the end of this
    int i = 0;
    int lsd = 0;
    char res = '\0';
    while(1) {
        lsd = value % 10;
        res = int_to_char(lsd);
        str[i] = res;
        ++i;
        value /= 10;
        if(value == 0) {
            // i gives count of number of characters in string

            // reverse string
            char tmp = NULL;
            int j = 0;
            for(;j<(i/2); j++) {
                tmp = str[j];
                str[j] = str[(i -1) -j];
                str[(i -1) -j] = tmp;
            }
            str[i] = '\0';
            return str;
        }
    }
}

int change_priority(pcb* in_pcb, PRIORITY_LEVEL in_plvl) {

#ifdef DEBUG 
    rtx_dbug_outs("[CHANGE PRIORITY] Changing ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" 's priority to ");
    rtx_dbug_out_int10(in_plvl);
	rtx_dbug_outs("\n\r");
#endif

    if(in_pcb == NULL) {

#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY] in_pcb = NULL\n\r");
#endif

        return RTX_ERROR;
    }
    if(!is_valid_priority(in_plvl)) {

#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY] Invalid priority\n\r");
#endif

        return RTX_ERROR;
    }

    // in_pcb != NULL
    // in_plvl is valid



#ifdef DEBUG
    rtx_dbug_outs("[CHANGE PRIORITY] Checking if ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" on ready_queue\n\r");
#endif

    pcb* res_pcb = NULL;
    res_pcb = dequeue_from_ready_queue(pcb_get_pid(in_pcb));
    if(res_pcb != NULL) {
        // is on ready queue

#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY] Re-enqueing ");
        rtx_dbug_outs(pcb_get_name(res_pcb));
        rtx_dbug_outs(" onto the ready_queue\n\r");
#endif

        return enque_onto_ready_queue(res_pcb, in_plvl);
    }



#ifdef DEBUG
    rtx_dbug_outs("[CHANGE PRIORITY] Checking if ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" on blocked_on_receive_queue\n\r");
#endif

    res_pcb = dequeue_from_blocked_on_receive_queue(pcb_get_pid(in_pcb));
    if(res_pcb != NULL) {
        // is on blocked_on_receive_queue

#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY] Re-enqueing ");
        rtx_dbug_outs(pcb_get_name(res_pcb));
        rtx_dbug_outs(" onto the blocked_on_receive_queue\n\r");
#endif

        return enque_onto_blocked_on_receive_queue(res_pcb, in_plvl);
    }



#ifdef DEBUG
    rtx_dbug_outs("[CHANGE PRIORITY] Checking if ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" on blocked_on_memory_queue\n\r");
#endif

    res_pcb = dequeue_from_blocked_on_memory_queue(pcb_get_pid(in_pcb));
    if(res_pcb != NULL) {
        // is on blocked_on_memory_queue

#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY] Re-enqueing ");
        rtx_dbug_outs(pcb_get_name(res_pcb));
        rtx_dbug_outs(" onto the blocked_on_memory_queue\n\r");
#endif

        return enque_onto_blocked_on_memory_queue(res_pcb, in_plvl);
    }

    // res_pcb == NULL

#ifdef DEBUG
    rtx_dbug_outs("[CHANGE PRIORITY] ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" not on any queue\n\r");
#endif

    if(pcb_get_pid(in_pcb) == NULL_PROCESS_PID) {
#ifdef DEBUG
        rtx_dbug_outs("[CHANGE PRIORITY][ERROR] Cannot change priority of Null process\n\r");
        return RTX_ERROR;
#endif
    }
    // me: check if i-process, and disallow change as well?

    // in_pcb not on any queue
    pcb_set_priority(in_pcb, in_plvl);

    return RTX_SUCCESS;
}

int envl_set_msgtext(envl* in_envl, char* in_msgtext) {
    if(in_envl == NULL) {
        return RTX_ERROR;
    }
    return copy_buffer(in_msgtext, in_envl->msgtext, ENVL_MSGTEXT_SIZE);
}

void print_ready_queue() {
    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        pcb_print_pcb_deque(&ready_queue[i]);
    }
}

void print_blocked_on_receive_queue() {
    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        pcb_print_pcb_deque(&blocked_on_receive_queue[i]);
    }
}

void print_blocked_on_memory_queue() {
    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        pcb_print_pcb_deque(&blocked_on_memory_queue[i]);
    }
}

void print_pcb_table() {
    int i = 0;
    for(; i<pcbcnt; i++) {
        if(pcb_table[i] != NULL) {
            pcb_print(pcb_table[i]);
        }
    }
}

void print_envl_table() {
    int i = 0;
    for(; i<envlcnt; i++) {
        if(envl_table[i] != NULL) {
            envl_print(envl_table[i]);
        }
    }
}

int is_pcb_on_ready_queue(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(is_pcb_on_deque(&ready_queue[i], in_pcb) == TRUE) {
#ifdef DEBUG
            rtx_dbug_outs("[IS_PCB_ON_READY_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(in_pcb));
            rtx_dbug_outs(" found on ready_queue\n\r");
#endif
            return TRUE;
        }
    }
#ifdef DEBUG
    rtx_dbug_outs("[IS_PCB_ON_READY_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" not found on ready_queue\n\r");
#endif
    return FALSE;
}

int is_pcb_on_blocked_on_receive_queue(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(is_pcb_on_deque(&blocked_on_receive_queue[i], in_pcb) == TRUE) {
#ifdef DEBUG
            rtx_dbug_outs("[IS_PCB_ON_BLOCKED_ON_RECEIVE_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(in_pcb));
            rtx_dbug_outs(" found on blocked_on_receive_queue\n\r");
#endif
            return TRUE;
        }
    }
#ifdef DEBUG
    rtx_dbug_outs("[IS_PCB_ON_BLOCKED_ON_RECEIVE_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" not found on blocked_on_receive_queue\n\r");
#endif
    return FALSE;
}

int is_pcb_on_blocked_on_memory_queue(pcb* in_pcb) {
    if(in_pcb == NULL) {
        return RTX_ERROR;
    }

    int i = 0;
    for(; i<NUM_PRIORITY; i++) {
        if(is_pcb_on_deque(&blocked_on_memory_queue[i], in_pcb) == TRUE) {
#ifdef DEBUG
            rtx_dbug_outs("[IS_PCB_ON_BLOCKED_ON_MEMORY_QUEUE] ");
            rtx_dbug_outs(pcb_get_name(in_pcb));
            rtx_dbug_outs(" found on blocked_on_memory_queue\n\r");
#endif
            return TRUE;
        }
    }
#ifdef DEBUG
    rtx_dbug_outs("[IS_PCB_ON_BLOCKED_ON_MEMORY_QUEUE] ");
    rtx_dbug_outs(pcb_get_name(in_pcb));
    rtx_dbug_outs(" not found on blocked_on_memory_queue\n\r");
#endif
    return FALSE;
}

int time_to_int(char* in_time) {
    if (in_time == NULL){
        return 0;
    }
    // assume well-formed, ie. "hh:mm:ss"
    char hh[2+1] = {'\0','\0','\0'};
    char mm[2+1] = {'\0','\0','\0'};
    char ss[2+1] = {'\0','\0','\0'};
    hh[0] = in_time[0];
    hh[1] = in_time[1];
    mm[0] = in_time[3];
    mm[1] = in_time[4];
    ss[0] = in_time[6];
    ss[1] = in_time[7];

    return 60*60*atoi(hh) +60*atoi(mm) +atoi(ss);
}

char* int_to_time(int in_time, char* in_buf) {
    // in_buf should have format hh:mm:ss (size >= 8+1)
    char tmpbuf[16];
    int hour = in_time / (60*60); // 60 m/h * 60 s/m
    in_time -= (hour*60*60);
    int min = in_time / 60; // 60 s/m
    in_time -= (min*60);
    int sec = in_time;

    itoa(hour, tmpbuf, 10);
    if(hour == 0) {
        in_buf[0] = '0';
        in_buf[1] = '0';
    }
    else if(hour >= 1 && hour <= 9) {
        in_buf[0] = '0';
        in_buf[1] = tmpbuf[0];
    }
    else {
        in_buf[0] = tmpbuf[0];
        in_buf[1] = tmpbuf[1];
    }
    in_buf[2] = ':';
    itoa(min, tmpbuf, 10);
    if(min == 0) {
        in_buf[3] = '0';
        in_buf[4] = '0';
    }
    else if(min >= 1 && min <= 9) {
        in_buf[3] = '0';
        in_buf[4] = tmpbuf[0];
    }
    else {
        in_buf[3] = tmpbuf[0];
        in_buf[4] = tmpbuf[1];
    }
    in_buf[5] = ':';
    itoa(sec, tmpbuf, 10);
    if(sec == 0) {
        in_buf[6] = '0';
        in_buf[7] = '0';
    }
    else if(sec >= 1 && sec <= 9) {
        in_buf[6] = '0';
        in_buf[7] = tmpbuf[0];
    }
    else {
        in_buf[6] = tmpbuf[0];
        in_buf[7] = tmpbuf[1];
    }
    in_buf[8] = '\0';
    return in_buf;
}
