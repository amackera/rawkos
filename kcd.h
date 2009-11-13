#ifndef KCD_H
#define KCD_H

#include "globalvar.h"
#include "func.h"
#include "ipc.h"
#include "mem.h"

typedef struct {
    char cmd;
    PROCESS_ID pid;
} cmd_entry;

#define MAX_CMD_COUNT 100
//const int MAX_CMD_COUNT = 100;

cmd_entry cmd_table[MAX_CMD_COUNT];

int check_parameters(char in_cmd, char* in_param);
BOOL is_command_char(char in_cmd);
//int verify_command(char* in_cmd);
PROCESS_ID search_command(char in_cmd);
int register_command(char in_cmd);
int process_registration(char in_cmd, PROCESS_ID in_pid);
void keyboard_command_decoder();

int print_cmd_table();

#endif /* KCD_H */
