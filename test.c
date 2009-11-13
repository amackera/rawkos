#include "test.h"

void pcb_deque_test() {
    pcb_deque* test_deque = (pcb_deque*)malloc(sizeof(pcb_deque));

    pcb* testpcb = (pcb*)malloc(sizeof(pcb));
    pcb* testpcb2 = (pcb*)malloc(sizeof(pcb));
    pcb* testpcb3 = (pcb*)malloc(sizeof(pcb));
    // mmap() // to specify size and location
    pcb_enque(testpcb, test_deque);
    pcb_enque(testpcb2, test_deque);
    pcb_enque(testpcb3, test_deque);

    pcb_dequeue_head(test_deque);
    pcb_dequeue_head(test_deque);
    pcb_dequeue_head(test_deque);

    pcb_enque(testpcb, test_deque);
    pcb_enque(testpcb2, test_deque);
    pcb_enque(testpcb3, test_deque);

    pcb_dequeue_tail(test_deque);
    pcb_dequeue_tail(test_deque);
    pcb_dequeue_tail(test_deque);

    pcb_enque(testpcb, test_deque);
    pcb_enque(testpcb2, test_deque);
    pcb_enque(testpcb3, test_deque);

    pcb_dequeue_head(test_deque);
    pcb_dequeue_tail(test_deque);
    pcb_dequeue_tail(test_deque);

    pcb_dequeue_tail(test_deque); // should throw error
}

void envl_deque_test() {
    envl_deque* test_deque = (envl_deque*)malloc(sizeof(envl_deque));

    envl* testenvl = (envl*)malloc(sizeof(envl));
    envl* testenvl2 = (envl*)malloc(sizeof(envl));
    envl* testenvl3 = (envl*)malloc(sizeof(envl));
    // mmap() // to specify size and location
    envl_enque(testenvl, test_deque);
    envl_enque(testenvl2, test_deque);
    envl_enque(testenvl3, test_deque);

    envl_dequeue_head(test_deque);
    envl_dequeue_head(test_deque);
    envl_dequeue_head(test_deque);

    envl_enque(testenvl, test_deque);
    envl_enque(testenvl2, test_deque);
    envl_enque(testenvl3, test_deque);

    envl_dequeue_tail(test_deque);
    envl_dequeue_tail(test_deque);
    envl_dequeue_tail(test_deque);

    envl_enque(testenvl, test_deque);
    envl_enque(testenvl2, test_deque);
    envl_enque(testenvl3, test_deque);

    envl_dequeue_head(test_deque);
    envl_dequeue_tail(test_deque);
    envl_dequeue_tail(test_deque);

    envl_dequeue_tail(test_deque); // should throw error
}

void wall_clock_test() {
    printf("parse_time(\"12:34:56\"): %d\n", parse_time("12:34:56"));
    printf("parse_time(\"12:34:56\"): %d\n", parse_time("45:67:89"));
}

void envelope_size_test() {
    envl* tmp_envl = (envl*)malloc(sizeof(envl));
    printf("sizeof(envl): %d\n", sizeof(envl));
}

void atoi_test() {
    printf("atoi(\"12345\"): %d\n", atoi("12345"));
    printf("atoi(\"12345A\"): %d\n", atoi("12345A"));
    printf("atoi(\"-12345\"): %d\n", atoi("-12345"));
    printf("atoi(\"\"): %d\n", atoi(""));
    printf("atoi(\"00010\"): %d\n", atoi("00010"));
    printf("atoi(\"10010\"): %d\n", atoi("10010"));
    char tmp[3] = {'0', '1', '0'};
    printf("atoi(\"{'0', '1', '0'}\"): %d\n", atoi(tmp));
    char tmp2[3] = {'0', '1', '1'};
    printf("atoi(\"{'0', '1', '1'}\"): %d\n", atoi(tmp2));
    char tmp3[4] = {'0', '1', '1',};
    printf("atoi(\"{'0', '1', '1',}\"): %d\n", atoi(tmp3));
    //printf("atoi(\"0x1234\"): %d\n", atoi(0x1234)); // segfaults
}

void bitshift_test() {
    long tmp_val = 0x12345678;
    printf("%x\n", tmp_val);
    UINT8 byte0 = 0;
    UINT8 byte1 = 0;
    UINT8 byte2 = 0;
    UINT8 byte3 = 0;
    byte0 = (tmp_val & 0x000000FF) >> (8 * 0);
    byte1 = (tmp_val & 0x0000FF00) >> (8 * 1);
    byte2 = (tmp_val & 0x00FF0000) >> (8 * 2);
    byte3 = (tmp_val & 0xFF000000) >> (8 * 3);
    printf("%x %x %x %x\n", byte3, byte2, byte1, byte0);
}

void itoa_test() {
    char str[33];
    int i = 0;
    for(; i<65536; i += 5) {
        printf("itoa(%d): %s\n", i, itoa(i, str, 10));
        if(i%50==0) {sleep(1);}
    }
}

void command_char_test() {
    printf("check_parameters(%s,%s): %d\n", "Z", "", check_parameters(*"Z",""));
    printf("check_parameters(%s,%s): %d\n", "Y", "", check_parameters(*"Y",""));
    printf("check_parameters(%s,%s): %d\n", "X", "", check_parameters(*"X",""));
    printf("check_parameters(%s,%s): %d\n", "Z", "garbage", check_parameters(*"Z","garbage"));
    printf("check_parameters(%s,%s): %d\n", "Y", "garbage", check_parameters(*"Y","garbage"));
    printf("check_parameters(%s,%s): %d\n", "X", "garbage", check_parameters(*"X","garbage"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 1", check_parameters(*"C","1 1"));
    printf("check_parameters(%s,%s): %d\n", "C", "10 1", check_parameters(*"C","10 1"));
    printf("check_parameters(%s,%s): %d\n", "C", "11 1", check_parameters(*"C","11 1"));
    printf("check_parameters(%s,%s): %d\n", "C", "1000 1", check_parameters(*"C","1000 1"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 -1", check_parameters(*"C","1 -1"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 3", check_parameters(*"C","1 3"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 4", check_parameters(*"C","1 4"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 100", check_parameters(*"C","1 100"));
    printf("check_parameters(%s,%s): %d\n", "C", "1 1 ", check_parameters(*"C","1 1 "));
    printf("check_parameters(%s,%s): %d\n", "W", "J", check_parameters(*"W","J"));
    printf("check_parameters(%s,%s): %d\n", "W", "T", check_parameters(*"W","T"));
    printf("check_parameters(%s,%s): %d\n", "W", "T ", check_parameters(*"W","T "));
    printf("check_parameters(%s,%s): %d\n", "W", "T     T", check_parameters(*"W","T     T"));
    printf("check_parameters(%s,%s): %d\n", "W", "TT", check_parameters(*"W","TT"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:45", check_parameters(*"W","S12:34:45"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:59:59", check_parameters(*"W","S12:59:59"));
    printf("check_parameters(%s,%s): %d\n", "W", "S32:34:45", check_parameters(*"W","S32:34:45"));
    printf("check_parameters(%s,%s): %d\n", "W", "S15:34:45", check_parameters(*"W","S15:34:45"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:64:45", check_parameters(*"W","S12:64:45"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:65", check_parameters(*"W","S12:34:65"));
    printf("check_parameters(%s,%s): %d\n", "W", "S", check_parameters(*"W","S"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:", check_parameters(*"W","S12:"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12!34!45", check_parameters(*"W","S12!34!45"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:45!", check_parameters(*"W","S12:34:45garbage"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:45 !", check_parameters(*"W","S12:34:45garbage"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:45garbage", check_parameters(*"W","S12:34:45garbage"));
    printf("check_parameters(%s,%s): %d\n", "W", "S12:34:45   ", check_parameters(*"W","S12:34:45   "));
}

void copy_buffer_test() {
    char src[] = "0123456789";
    char src2[] = {'1','2'};
    char src3[] = {'1','2',0};
    char dest[15];
    char dest2[5];
    copy_buffer(src, dest, 15);
    printf("copy_buffer(\"0123456789\", dest, 15): %s\n", dest);
    copy_buffer(src2, dest, 15);
    printf("copy_buffer({\'1\',\'2\'}, dest, 15): %s\n", dest);
    copy_buffer(src3, dest, 15);
    printf("copy_buffer({\'1\',\'2\',0}, dest, 15): %s\n", dest);
    copy_buffer(src, dest2, 5);
    printf("copy_buffer(\"0123456789\", dest, 5): %s\n", dest2);
}

void msglog_test() {
    msglog send_log;
    send_log.entry_cnt = 0;
    send_log.startpos = 0;
    msglog receive_log;
    receive_log.entry_cnt = 0;
    receive_log.startpos = 0;
    envl envl1;
    envl envl2;
    envl envl3;
    envl_clear(&envl1);
    envl_clear(&envl2);
    envl_clear(&envl3);

    envl1.sender_pid = 1;
    envl1.destination_pid = 2;
    envl1.msg_type = 1;
    envl1.sent_time = 1;
    envl1.sent_time = 10;
    envl_set_msgtext(&envl1, "012345678901234567890123456789");

    envl2.sender_pid = 2;
    envl2.destination_pid = 3;
    envl2.msg_type = 2;
    envl2.sent_time = 2;
    envl2.sent_time = 20;
    envl_set_msgtext(&envl2, "012345678901234567890123456789");

    envl3.sender_pid = 3;
    envl3.destination_pid = 4;
    envl3.msg_type = 3;
    envl3.sent_time = 3;
    envl3.sent_time = 30;
    envl_set_msgtext(&envl3, "012345678901234567890123456789");

    msglog_save_transaction(&send_log, &envl1);
    msglog_save_transaction(&send_log, &envl2);
    msglog_save_transaction(&send_log, &envl3);

    msglog_print(&send_log);
}

int main() {

    //    pcb_deque_test();

    //    envl_deque_test();

    // envelope_size_test();

    // wall_clock_test();

    //atoi_test();

    //bitshift_test();

    //itoa_test();

    //command_char_test();

    //copy_buffer_test();

    msglog_test();

    //init();

    return 0;
}
