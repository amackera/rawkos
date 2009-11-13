#ifndef FUNC_H
#define FUNC_H

#include "globalvar.h"
#include "outs.h"

// Contains all functions that operate on globalvar.h

pcb* get_pcb(PROCESS_ID in_pid);
pcb* peek_pcb(PROCESS_ID in_pid);

envl* get_envl_by_sender_pid(pcb* in_pcb, PROCESS_ID* in_pid);

envl* dequeue_from_free_mem_queue();
int enque_onto_free_mem_queue(envl* in_envl);
envl* dequeue_from_blocked_mem_queue();
int enque_onto_blocked_mem_queue(envl* in_envl);
envl* dequeue_from_delay_send_queue();
int enque_onto_delay_send_queue(envl* in_envl);
int enque_onto_delay_send_queue_by_delivery_time(envl* in_envl);

int clear_buffer(char* in_buf, int count);
int copy_buffer(char* in_src_buf, char* in_dest_buf, int in_num);
//int set_process_state(pcb* in_proc, PROCESS_STATE in_state);

int enque_onto_ready_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl);
int enque_onto_blocked_on_receive_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl);
int enque_onto_blocked_on_memory_queue(pcb* in_pcb, PRIORITY_LEVEL in_plvl);
pcb* dequeue_from_ready_queue(PROCESS_ID in_pid);
pcb* dequeue_from_blocked_on_receive_queue(PROCESS_ID in_pid);
pcb* dequeue_from_blocked_on_memory_queue(PROCESS_ID in_pid);

int atomic(int in_state);
int char_to_int(char in_num);
int atoi(char* in_string);
char int_to_char(int in_num);
char* itoa(int value, char* str, int base);

int change_priority(pcb* in_pcb, PRIORITY_LEVEL in_plvl);
int envl_set_msgtext(envl* in_envl, char* in_msgtext);

void print_ready_queue();
void print_blocked_on_receive_queue();
void print_blocked_on_memory_queue();
void print_pcb_table();
void print_envl_table();

int is_pcb_on_ready_queue(pcb* in_pcb);
int is_pcb_on_blocked_on_receive_queue(pcb* in_pcb);
int is_pcb_on_blocked_on_memory_queue(pcb* in_pcb);

int time_to_int(char* in_time);
char* int_to_time(int in_time, char* in_buf);

#endif /* FUNC_H */
