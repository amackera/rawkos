#include "const.h"

BOOL is_valid_pid(PROCESS_ID in_pid) {
    return !(in_pid < MIN_PID || in_pid > MAX_PID);
}

BOOL is_valid_priority(PRIORITY_LEVEL in_plvl) {
    return !(in_plvl < MIN_PRIORITY_LEVEL || in_plvl > MAX_PRIORITY_LEVEL);
}

BOOL is_valid_state(PROCESS_STATE in_pstate) {
    return !(in_pstate < MIN_PROCESS_STATE || in_pstate > MAX_PROCESS_STATE);
}

BOOL is_valid_systime(SYSTIME in_time) {
    return (in_time >= 0 && in_time < MAX_SYSTIME);
}

BOOL is_valid_message_type(MESSAGE_TYPE in_msgtype) {
    return !(in_msgtype < MIN_MESSAGE_TYPE || in_msgtype > MAX_MESSAGE_TYPE);
}

char* get_process_state_name(PROCESS_STATE in_state) {
    switch(in_state) {
    case READY: return "ready"; break;
    case RUNNING: return "running"; break;
    case BLOCKED_ON_MEMORY: return "blocked on memory"; break;
    case BLOCKED_ON_MESSAGE: return "blocked on message"; break;
    case I_PROC: return "i-proc"; break;
    default: return "unknown"; break;
    }
}

char* get_message_type_name(MESSAGE_TYPE in_msgtype) {
    switch(in_msgtype) {
    case KEYBOARD_INPUT: return "keyboard input"; break;
    case KCD_REGISTER: return "kcd register"; break;
    case KCD_MSG: return "kcd msg"; break;
    case TEXT_MSG: return "text msg"; break;
    case MISC_MSG: return "misc msg"; break;
    case CRT_DISPLAY: return "crt display"; break;
    case COUNT_REPORT: return "count report"; break;
    case WAKEUP10: return "wakeup10"; break;
    default: return "unknown"; break;
    }
}
