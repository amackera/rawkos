#ifndef CONST_H
#define CONST_H

#ifdef _RTX_EVAL_
#include "rtxeval_dummy.h"
#endif

#include "rtx_inc.h"

// Types of PROCESS_STATE
#define INVALID_PROCESS_STATE -1
#define READY 1 // Process on ready queue
#define MIN_PROCESS_STATE READY
#define RUNNING 2 // Process currently executing
#define BLOCKED_ON_MEMORY 3 // Process waiting for an envelope
#define BLOCKED_ON_MESSAGE 4 // Process waiting for another process's message
#define I_PROC 5  // Not running state for i_processes
#define MAX_PROCESS_STATE I_PROC

// Types of MESSAGE_TYPE
#define INVALID_MESSAGE_TYPE -1
#define KEYBOARD_INPUT 0
#define MIN_MESSAGE_TYPE KEYBOARD_INPUT
#define KCD_REGISTER 1
#define KCD_MSG 2
#define TEXT_MSG 3
#define MISC_MSG 4
#define CRT_DISPLAY 5
// for process C1
#define COUNT_REPORT 6
#define WAKEUP10 7
#define MAX_MESSAGE_TYPE WAKEUP10

// Process IDs
#define INVALID_PID -1

#define MIN_PID 1
// PIDs 1-6 reserved for testing processes
#define A1_PID 7
#define B1_PID 8
#define C1_PID 9
#define A2_PID 10
#define B2_PID 11
#define C2_PID 12
#define A3_PID 13

#define NULL_PROCESS_PID 14
#define KCD_PID 15
#define CRT_DISPLAY_PID 16
#define TIMER_I_PID 17
#define UART_I_PID 18
#define SPC_PID 19
#define WALL_CLOCK_PID 20
#define MAX_PID WALL_CLOCK_PID

// Misc
// me: success/error messages should have its own type
#define RTX_SUCCESS 0 // Code for successful procedure execution
#define RTX_ERROR -1

#define MEM_BLOCK_USED 1
#define MEM_BLOCK_UNUSED 0

#define INVALID_PRIORITY_LEVEL -1
#define PRIORITY_LEVEL_0 0
#define MIN_PRIORITY_LEVEL PRIORITY_LEVEL_0
#define PRIORITY_LEVEL_1 1
#define PRIORITY_LEVEL_2 2
#define PRIORITY_LEVEL_3 3
#define PRIORITY_LEVEL_4 4 // For null process only
#define MAX_PRIORITY_LEVEL PRIORITY_LEVEL_4
#define NUM_PRIORITY 5

#define ON 1
#define OFF 0

// current_primitive; used to decide which kernel level handler to call
#define SEND_MESSAGE_PRIMITIVE 1
#define RECEIVE_MESSAGE_PRIMITIVE 2
#define REQUEST_MEMORY_BLOCK_PRIMITIVE 3
#define RELEASE_MEMORY_BLOCK_PRIMITIVE 4
#define RELEASE_PROCESSOR_PRIMITIVE 5
#define DELAYED_SEND_PRIMITIVE 6
#define SET_PROCESS_PRIORITY_PRIMITIVE 7
#define GET_PROCESS_PRIORITY_PRIMITIVE 8
#define RTX_NORMAL 9
#define RTX_INIT 10
#define CONTEXT_SWITCH_TIMER 11
#define CONTEXT_SWITCH_UART 12

// "typedef"s
// The fields sender_process_id, destination_process_id and message_type are all of type int (me: hence PROCESS_ID and MESSAGE_TYPE are #define 'd as int
#define PRIORITY_LEVEL int
#define PROCESS_STATE int
#define PROCESS_ID int 
#define SYSTIME int
#define MAX_SYSTIME 1000000
#define MESSAGE_TYPE int
#define ADDR_PTR void* // todo: find correct type and use name ADDR instead
#define BOOL BOOLEAN

#define DEFAULT_PCBCNT MAX_PID
#define DEFAULT_ENVLCNT 35

// system time has units of seconds
#define CLK_UNIT 1

BOOL is_valid_pid(PROCESS_ID in_pid);
BOOL is_valid_priority(PRIORITY_LEVEL in_plvl);
BOOL is_valid_state(PROCESS_STATE in_pstate);
BOOL is_valid_systime(SYSTIME in_time);
BOOL is_valid_message_type(MESSAGE_TYPE in_msgtype);

char* get_process_state_name(PROCESS_STATE in_state);
char* get_message_type_name(MESSAGE_TYPE in_msgtype);

#endif /* CONST_H */
