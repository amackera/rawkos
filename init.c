#include "init.h"

void init_swi_handler() {
#ifndef ASM_OFF
    //Setting up the vector trap #6 (0x80 + # *4)
    asm("move.l %d0, -(%a7)");
    asm("move.l #asm_trap_entry, %d0");
    asm("move.l %d0, 0x10000098");
    
    asm("move.l (%a7)+, %d0");
#endif
}

void init_timer_interrupt() {
	UINT32 interrupt_mask;

#ifndef ASM_OFF
	asm("move.l %D0,-(%A7)");
	asm("move.l #asm_timer_entry,%D0");
	asm("move.l %D0,0x10000078");
	asm("move.l (%A7)+,%D0");

	//clear the timer event register
	TIMER0_TER = 0x03;	

	//setup timer interrupt controller register
	//bit 1-0 = IP = 10 -> interrupt priority (high)
	//bit 4-2 = IL = 110 -> interrupt level (6)
	//bit 6-5 = 00 -> reserved
	//bit 7 = AVEC = 1 -> autovector enable generating SIM during interrupt ack cycle 
	TIMER0_ICR = 0x9B;

	//set to approx 1s
	//(1/45)x(16)x(256+1)(2AEA = 10986) = 1.0039s
   	//TIMER0_TRR = 0xD5BD; // 5 seconds
    	//TIMER0_TRR = 0x26D0; // <1 second
//    	TIMER0_TRR = 0x2AEA; // 1 second
	TIMER0_TRR = 175; // 10 ms

	//setup the timer mode stuff
	//bit 0 = RST = 1 -> enable timer
	//bit 2-1 = CLK = 01 -> system bus clock divided by 16
	//bit 3 = FRR = 1 -> restart. timer counter is reset after reaching ref
	//bit 4 = ORI = 1 -> enable interrupt upon reaching the reference value
	//bit 5 = OM = 1 -> toggle output
	//bit 7-6 = CE = 00 -> disable interrupt on capture event
	//bit 15-8 = PS = 0xFF -> prescaler to divide the clock by 256
	TIMER0_TMR = 0xFF3D;

	//mask the timer interrupt
	interrupt_mask = SIM_IMR;
	interrupt_mask &= 0x0003fdff;
	SIM_IMR = interrupt_mask;
#endif
}

void init_uart_interrupt() {
	UINT32 interrupt_mask;

#ifndef ASM_OFF
	//UART init
	asm("move.l %D0,-(%A7)");
	asm("move.l #asm_serial_entry,%D0");
	asm("move.l %D0,0x10000100");
	asm("move.l (%A7)+,%D0");

    	// Reset the Entire UART
	SERIAL1_UCR = 0x10;
	// Reset the receiever
	SERIAL1_UCR = 0x20;
    	// Reset the transmitter
	SERIAL1_UCR = 0x30;
    	// Reset the error condition
	SERIAL1_UCR = 0x40;

	//setup UART interrupt controller register
	//bit 1-0 = IP = 10 -> interrupt priority (high)
	//bit 4-2 = IL = 110 -> interrupt level (6)
	SERIAL1_ICR = 0x17; //9A;
	
	//set UART interrupt vector registor
	SERIAL1_IVR = 0x40;

	//set the UART interrupt mask register	
	SERIAL1_IMR = 0x02;

	//setup UART to transmit and receive
	SERIAL1_UCR = 0x05;

	//mask the UART interrupt
	interrupt_mask = SIM_IMR;
	interrupt_mask &= 0x0003dfff;
	SIM_IMR = interrupt_mask;
#endif
}

/*
void init_pcb_init_table() {

    int i = 0;

#ifdef _RTX_EVAL_
    setTestProcs();

    for (; i < NUM_TEST_PROCS; i++) {
		pcb_init_table[i].pid = RTXEVAL_TEST_PROCS[i].pid;
		pcb_init_table[i].priority = RTXEVAL_TEST_PROCS[i].priority;
		pcb_init_table[i].stack_size = RTXEVAL_TEST_PROCS[i].stacksize;
		pcb_init_table[i].code_pointer = RTXEVAL_TEST_PROCS[i].rtxeval_proc_entry;
	}
#endif

    pcb_init_table[i].pid = NULL_PROCESS_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_4;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &null_process;
    ++i;

    pcb_init_table[i].pid = A1_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_a1;
    ++i;

    pcb_init_table[i].pid = B1_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_b1;
    ++i;

    pcb_init_table[i].pid = C1_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_c1;
    ++i;

    pcb_init_table[i].pid = A2_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_a2;
    ++i;

    pcb_init_table[i].pid = B2_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_b2;
    ++i;
    
    pcb_init_table[i].pid = C2_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_c2;
    ++i;

    pcb_init_table[i].pid = A3_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_a3;
    ++i;

    pcb_init_table[i].pid = A3_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &process_a3;
    ++i;

    pcb_init_table[i].pid = KCD_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &keyboard_command_decoder;
    ++i;

    pcb_init_table[i].pid = CRT_DISPLAY_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_3;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &crt_display;
    ++i;

    pcb_init_table[i].pid = TIMER_I_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_0;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &timer_i_process;
    ++i;

    pcb_init_table[i].pid = UART_I_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_0;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &uart_i_process;
    ++i;

    pcb_init_table[i].pid = SPC_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_0;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &set_priority_command_process;
    ++i;

    pcb_init_table[i].pid = WALL_CLOCK_PID;
    pcb_init_table[i].priority = PRIORITY_LEVEL_0;
    pcb_init_table[i].stack_size = ; // todo: decide on stack size
    pcb_init_table[i].code_pointer = &wall_clock_display_process;
    ++i;
}
*/

void clear_block(void* in_start_addr, void* in_end_addr) {

    // check that start_addr <= in_end_addr; otherwise, return
    if(in_start_addr > in_end_addr) {
        return;
    }

    void* tmp_addr = in_start_addr;
    while(tmp_addr >= in_end_addr) {
        *((char*)tmp_addr) = 0x00;
        tmp_addr += 1; // todo: get size of increment
    }
}

void set_initial_stack_state(pcb* in_pcb) {

    /*
      non i-processes
      ================ top of memory block
      .
      .
      .

      addr x16 (16 x 4 bytes) = 0x00000000 <-- SP
      SR (2 bytes) = 0x0000
      PC (4 bytes) = code pointer
      ================ bottom of memory block (and bottom of stack)

      i-processes
      ================ top of memory block
      .
      .
      .

      addr x16 (16 x 4 bytes) = 0x00000000 <-- SP
      SR (2 bytes) = 0x2700
      PC (4 bytes) = code pointer
      ================ bottom of memory block (and bottom of stack)
    */
  
    void *stack_ptr = NULL;
    void *proc_code = NULL;
    SINT32 proc_sr = 0;

    stack_ptr = in_pcb->stack_pointer;
    proc_code = in_pcb->code_pointer;
    proc_sr = in_pcb->exception_stack_frame;
    /*
      #ifdef DEBUG
      rtx_dbug_outs("[SET_INITIAL_STACK_STATE] Local stack_ptr = ");
      rtx_dbug_out_int16(stack_ptr);
      rtx_dbug_outs("\n\r");

      rtx_dbug_outs("[SET_INITIAL_STACK_STATE] Real stack_pointer = ");
      rtx_dbug_out_int16(in_pcb->stack_pointer);
      rtx_dbug_outs("\n\r");
  

      #endif
    */

    // We use A0 as a temporary stack pointer
    asm("move.l %a0, -(%a7)");
    // We utilize D0 in this set of instructions, therefore save it
    asm("move.l %d0, -(%a7)");

    asm("move.l -4(%a6), %a0");   //A0 points to in_pcb->stack_pointer
    asm("move.l -8(%a6), %d0");
    asm("move.l %d0, -(%a0)");
    asm("move.l -12(%a6), %d0");
    asm("move.l %d0, -(%a0)");

    // Initialize these bad boys to zero
    asm("move.l #0, -(%a0)"); //d0
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //d7
    asm("move.l #0, -(%a0)"); //a0
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //.
    asm("move.l #0, -(%a0)"); //a6

    // This is so context_swich() doesn't explode
    asm("move.l #asm_trap_exit, %d0");
    asm("move.l %d0, -(%a0)");
  
    asm("move.l #0, -(%a0)");
    asm("move.l %a0, %d0");
  
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l #0, -(%a0)"); 
    asm("move.l %d0, -(%a0)");
  
    asm("move.l %a0, -4(%a6)");

    in_pcb->stack_pointer = stack_ptr;

    asm("move.l (%a7)+, %d0");
    asm("move.l (%a7)+, %a0");
  
    return;
  
    /*
      pcb_set_stack_pointer(in_pcb, pcb_get_stack_end_addr(in_pcb)); // start at bottom

 
      void* tmp_sp = pcb_get_stack_end_addr(in_pcb);
      void* tmp_cp = pcb_get_code_pointer(in_pcb);

      tmp_sp -= 4 +1; // todo: check size of decrement // note: sizeof coldfire address is 32-bit (4 bytes) // +1 since boundry byte location is writeable (and hence is part of the stack)

      asm("nop");
      // set bottom of stack to code pointer
      *((long*)tmp_sp) = (long)tmp_cp;

      tmp_sp -= 4;

      // MS16bits are format, FS[3-2], VEC, and FS[1-0]
      // LS16bits are SR
      if(pcb_is_i_process(in_pcb) == TRUE) {
      *((long*)tmp_sp) = 0x40800000
      }
      else {
      *((long*)tmp_sp) = 0x40800000;
      }
    
      int i = 0;
      for(; i<4*16; i++) { // note: sizeof(long_{coldfire}) = 4 Bytes
      --tmp_sp;
      *((char*)tmp_sp) = 0x00;
      }
    
      asm("move.l #asm_trap_exit, %d0");
      asm("move.l %d0, -4(%a6)");
    
      tmp_sp -= 4;

      int j = 0;
      for(; j<4*16; j++) { // note: sizeof(long_{coldfire}) = 4 Bytes
      --tmp_sp;
      *((char*)tmp_sp) = 0x0;
      }

      pcb_set_stack_pointer(in_pcb, (char*)tmp_sp); // might cause a problem going from long* to char*
    */
}

int init() {
    //extern void * pcbStartAddress;
    //extern void * __end;

 
#ifndef ASM_OFF
    void* data_struct_start_addr = &__end;
#else
    // since __end undefined on my machine
    // 1 << 10 = 2^{20} B = 1 MB since OS needs 1 MB of space
    //void* __start = malloc(1<<20);
    void* __start = malloc(1<<(10+5)); // 32 k
    void* data_struct_start_addr = __start;
    // 
#endif

    // ================================================================
    // Initialize hardware, then interrupts

    init_swi_handler();
    init_timer_interrupt();
    init_uart_interrupt();

    // ================================================================
    // Global variables initialize

    // me: do we need to initialize them here? We could just set these default values when they are declared
    pcbcnt = 0;
    envlcnt = 0;
    clock_state = FALSE; // ie. OFF
    currtime = 0;
    cmd_table_cnt = 0;
    delaySend = FALSE;
    delaytime = 0;

    // ================================================================
    // Data structures allocation

    void* data_struct_end_addr = data_struct_start_addr;

    // note: these priority queues fixed to 5 levels
    int pcb_pdeque_size = sizeof(pcb_deque)*NUM_PRIORITY;
    int a = 0;
    for(; a<NUM_PRIORITY; a++) {
        blocked_on_receive_queue[a].head = NULL;
        blocked_on_receive_queue[a].tail = NULL;
        blocked_on_memory_queue[a].head = NULL;
        blocked_on_memory_queue[a].tail = NULL;
        ready_queue[a].head = NULL;
        ready_queue[a].tail = NULL;        
    }
    //    blocked_on_receive_queue = (pcb_deque*)data_struct_end_addr;
    data_struct_end_addr += pcb_pdeque_size;
    //    blocked_on_memory_queue = (pcb_deque*)data_struct_end_addr;
    data_struct_end_addr += pcb_pdeque_size;
    //    ready_queue = (pcb_deque*)data_struct_end_addr;
    data_struct_end_addr += pcb_pdeque_size;

    int envl_deque_size = sizeof(envl_deque);
    free_mem_queue = data_struct_end_addr;
    free_mem_queue->head = NULL;
    free_mem_queue->tail = NULL;
    free_mem_queue_cnt = 0;
    data_struct_end_addr += envl_deque_size;
    blocked_mem_queue = data_struct_end_addr;
    blocked_mem_queue->head = NULL;
    blocked_mem_queue->tail = NULL;
    blocked_mem_queue_cnt = 0;
    data_struct_end_addr += envl_deque_size;
    delay_send_queue = data_struct_end_addr;
    delay_send_queue->head = NULL;
    delay_send_queue->tail = NULL;
    delay_send_queue_cnt = 0;
    data_struct_end_addr += envl_deque_size;

    // ================================================================
    // PCB allocation

    void* pcb_start_addr = data_struct_end_addr +2;
    
    // Fill table with addresses of PCBs
    int pcb_size = sizeof(pcb);
    int i = 0;
    for(; i<DEFAULT_PCBCNT; i++) {
        pcb_table[i] = (pcb_start_addr +(pcb_size * i));
    }
    void* pcb_end_addr = (pcb_start_addr +(pcb_size * i));

    // ================================================================
    // Memory (envelopes) allocation

    void* envl_start_addr = pcb_end_addr +2;

    int envl_size = sizeof(envl);

    int j = 0;
    envl* new_envl = NULL;
    for(; j<DEFAULT_ENVLCNT; j++) {
        // allocating envelopes sequentially
        new_envl = (envl*)(envl_start_addr +(envl_size * j));

        // keeping track of them through the table
        envl_table[j] = new_envl;
        ++envlcnt;

        // enque all envelopes onto the free_mem_queue
        enque_onto_free_mem_queue(new_envl);

        envl_clear(new_envl);
    }

    void* envl_end_addr = (envl_start_addr +(envl_size * j));

    // ================================================================
    // Process stack allocation

    void* proc_stack_start_addr = envl_end_addr +2;
    void* proc_stack_end_addr = proc_stack_start_addr;

    int proc_stack_size = 1<<15; // 32 kiB// todo: find out size of actual process stack

    int k = 0;




    // assume pcbcnt = 0 here
#ifdef _RTX_EVAL_
    // for automated test processes
    // allocate stacks and pcbs
    setTestProcs();

    for(; k<NUM_TEST_PROCS; k++) {
        pcb_set_stack_start_addr(pcb_table[pcbcnt],proc_stack_end_addr);
        proc_stack_end_addr += RTXEVAL_TEST_PROCS[pcbcnt].stacksize; // note: it is an error to add two void* (they should be cast to int or something first)
        pcb_set_stack_end_addr(pcb_table[pcbcnt],proc_stack_end_addr);
        proc_stack_end_addr += 4 +2; // the "buffer" space between stacks (me: I don't think any address is longer than long)
        clear_block(pcb_get_stack_start_addr(pcb_table[pcbcnt]), pcb_get_stack_end_addr(pcb_table[pcbcnt]));
        pcb_set_stack_pointer(pcb_table[pcbcnt], pcb_get_stack_end_addr(pcb_table[pcbcnt]));
        //
        pcb_table[pcbcnt]->pid = RTXEVAL_TEST_PROCS[pcbcnt].pid;
        pcb_table[pcbcnt]->priority = RTXEVAL_TEST_PROCS[k].priority;
        pcb_table[pcbcnt]->state = READY;
        pcb_table[pcbcnt]->i_process = FALSE;
        pcb_table[pcbcnt]->messages.head = NULL;
        pcb_table[pcbcnt]->messages.tail = NULL;
        pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
        pcb_table[pcbcnt]->stack_size = RTXEVAL_TEST_PROCS[k].stacksize;
        enque_onto_ready_queue(pcb_table[pcbcnt], pcb_get_priority(pcb_table[pcbcnt]));
        pcb_set_code_pointer(pcb_table[pcbcnt], RTXEVAL_TEST_PROCS[k].rtxeval_proc_entry);
        set_initial_stack_state(pcb_table[pcbcnt]);
        pcbcnt++;
    }
    // assuming NUM_TEST_PROCS == 6
    pcb_table[0]->name = "rtx_test1";
    pcb_table[1]->name = "rtx_test2";
    pcb_table[2]->name = "rtx_test3";
    pcb_table[3]->name = "rtx_test4";
    pcb_table[4]->name = "rtx_test5";
    pcb_table[5]->name = "rtx_test6";

    // assert k == NUM_TEST_PROCS
    // assert pcbcnt = NUM_TEST_PROCS
#endif

    for(; k < DEFAULT_PCBCNT; k++) {

        /*
          note:
          end of block k: N
          start of block k +1: N + buffer size +1
          This means that anything in the address range [N +1, N +buffer size] does not belong to any process, and should not be written to. This also implies that at least 1 byte will seperate the boundry between the end of block k and the start of block k +1.
          For reasons of simplier math I think, the actual contents of the boundry locations should not be used. Therefore, [N, N +buffer size +1] should be used by nobody (me: OR perhaps those 1 bytes (me: >1?) should be occupied with some sorta of special boundry byte sequence or something like that.
        */

        pcb_set_stack_start_addr(pcb_table[k],proc_stack_end_addr);
        proc_stack_end_addr += (proc_stack_size * (k +1)); // note: it is an error to add two void* (they should be cast to int or something first)
        pcb_set_stack_end_addr(pcb_table[k],proc_stack_end_addr);
        proc_stack_end_addr += 4 +2; // the "buffer" space between stacks (me: I don't think any address is longer than long)
        clear_block(pcb_get_stack_start_addr(pcb_table[k]), pcb_get_stack_end_addr(pcb_table[k]));

        pcb_set_stack_pointer(pcb_table[k], pcb_get_stack_end_addr(pcb_table[k])); // start at bottom
    }
    //    void* proc_stack_end_addr = (proc_stack_start_addr +(proc_stack_size * k));

    // ================================================================
    // Reserved space allocation

    // ================================================================
    // Initialize PCBs



    pcb_table[pcbcnt]->pid = NULL_PROCESS_PID;
    pcb_table[pcbcnt]->name = "Null process";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_4;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    pcb_enque(pcb_table[pcbcnt], &ready_queue[PRIORITY_LEVEL_4]);
    pcb_set_code_pointer(pcb_table[pcbcnt], &null_process);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    /*
      pcb_table[pcbcnt]->pid = TEST_1_PID;
      pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
      pcb_table[pcbcnt]->state = READY;
      pcb_table[pcbcnt]->i_process = FALSE;
      pcb_table[pcbcnt]->messages.head = NULL;
      pcb_table[pcbcnt]->messages.tail = NULL;
      pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
      enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
      pcb_set_code_pointer(pcb_table[pcbcnt], &test_1);
      set_initial_stack_state(pcb_table[pcbcnt]);
      pcbcnt++;
    */

    // PIDs 1-6 reserved for testing processes

    pcb_table[pcbcnt]->pid = A1_PID;
    pcb_table[pcbcnt]->name = "A1";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_a1);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = B1_PID;
    pcb_table[pcbcnt]->name = "B1";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_b1);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = C1_PID;
    pcb_table[pcbcnt]->name = "C1";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_c1);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = A2_PID;
    pcb_table[pcbcnt]->name = "A2";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_a2);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = B2_PID;
    pcb_table[pcbcnt]->name = "B2";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_b2);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = C2_PID;
    pcb_table[pcbcnt]->name = "C2";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_c2);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = A3_PID;
    pcb_table[pcbcnt]->name = "A3";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_3;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &process_a3);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = KCD_PID;
    pcb_table[pcbcnt]->name = "KCD";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_0;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_0);
    pcb_set_code_pointer(pcb_table[pcbcnt], &keyboard_command_decoder);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = CRT_DISPLAY_PID;
    pcb_table[pcbcnt]->name = "CRT";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_0;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    pcb_table[pcbcnt]->messages.tail = NULL;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_0);
    pcb_set_code_pointer(pcb_table[pcbcnt], &crt_display);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = TIMER_I_PID;
    pcb_table[pcbcnt]->name = "Timer";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_0;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = TRUE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40802700;
    //enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &timer_i_process);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = UART_I_PID;
    pcb_table[pcbcnt]->name = "UART";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_0;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = TRUE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40802700;
    //enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_3);
    pcb_set_code_pointer(pcb_table[pcbcnt], &uart_i_process);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = SPC_PID;
    pcb_table[pcbcnt]->name = "SPC";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_1;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_1);
    pcb_set_code_pointer(pcb_table[pcbcnt], &set_priority_command_process);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;

    pcb_table[pcbcnt]->pid = WALL_CLOCK_PID;
    pcb_table[pcbcnt]->name = "Wall clock";
    pcb_table[pcbcnt]->priority = PRIORITY_LEVEL_1;
    pcb_table[pcbcnt]->state = READY;
    pcb_table[pcbcnt]->i_process = FALSE;
    pcb_table[pcbcnt]->messages.head = NULL;
    pcb_table[pcbcnt]->messages.tail = NULL;
    pcb_table[pcbcnt]->exception_stack_frame = 0x40800000;
    enque_onto_ready_queue(pcb_table[pcbcnt], PRIORITY_LEVEL_1);
    pcb_set_code_pointer(pcb_table[pcbcnt], &wall_clock_display_process);
    set_initial_stack_state(pcb_table[pcbcnt]);
    pcbcnt++;






      
    //current_process = get_pcb(NULL_PROCESS_PID);

#ifdef SPLASH
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("    @@@@@@@    @@@@@@   @@@  @@@  @@@  @@@  @@@  \n\r");
    rtx_dbug_outs("    @@@@@@@@  @@@@@@@@  @@@  @@@  @@@  @@@  @@@  \n\r");
    rtx_dbug_outs("    @@!  @@@  @@!  @@@  @@!  @@!  @@!  @@!  !@@  \n\r");
    rtx_dbug_outs("    !@!  @!@  !@!  @!@  !@!  !@!  !@!  !@!  @!!  \n\r");
    rtx_dbug_outs("    @!@!!@!   @!@!@!@!  @!!  !!@  @!@  @!@@!@!   \n\r");
    rtx_dbug_outs("    !!@!@!    !!!@!!!!  !@!  !!!  !@!  !!@!!!    \n\r");
    rtx_dbug_outs("    !!: :!!   !!:  !!!  !!:  !!:  !!:  !!: :!!   ,---.,---. \n\r");
    rtx_dbug_outs("    :!:  !:!  :!:  !:!  :!:  :!:  :!:  :!:  !:!  |   |`---. \n\r");
    rtx_dbug_outs("    ::   :::  ::   :::   :::: :: :::    ::  :::  |   |    | \n\r");
    rtx_dbug_outs("     :   : :   :   : :    :: :  : :     :   :::  `---'`---' \n\r");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("INITIALIZING MEMORY STACK FRAMES.....................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("ROOT USER LOG IN.....................................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("KERNEL PANIC!........................................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("ASSUME THE PARTY POSITION............................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("THE CAKE IS A LIE....................................[FAILED]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("CRITICAL ERROR - CORE DUMP IMMINENT..................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("INITIALIZING BLUE SCREEN OF DEATH....................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("OOPS THE SYSTEM WAS UNABLE TO PERFORM YOUR OPERATION [SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("ERROR IS ERROR.......................................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("HACK.EXE.............................................[SUCCESS]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("THE OPERATION COMPLETED SUCCESSFULLY.................[FAILED]\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    rtx_dbug_outs("\r\n");
    sleep(3);

    int l = 0;
    for(; l < pcbcnt; l++){
        rtx_dbug_outs("======== INITIALIZE PCB STACK ========\n\r");

        rtx_dbug_outs("    "); 
        rtx_dbug_outs(pcb_get_name(pcb_table[l]));
        rtx_dbug_outs("\n\r"); 

        rtx_dbug_outs("  pid = ");
        rtx_dbug_out_int10(pcb_get_pid(pcb_table[l]));
        rtx_dbug_outs("\n\r");

        rtx_dbug_outs("  stack_start_addr = ");
        rtx_dbug_out_int16((UINT32)pcb_get_stack_start_addr(pcb_table[l]));
        rtx_dbug_outs("\n\r");
    
        rtx_dbug_outs("  stack_end_addr = ");
        rtx_dbug_out_int16((UINT32)pcb_get_stack_end_addr(pcb_table[l]));
        rtx_dbug_outs("\n\r");
    
        rtx_dbug_outs("  stack_pointer = ");
        rtx_dbug_out_int16((UINT32)pcb_get_stack_pointer(pcb_table[l]));
        rtx_dbug_outs("\n\r");

        rtx_dbug_outs("  i_process = ");
        rtx_dbug_out_int16(pcb_is_i_process(pcb_table[l]));
        rtx_dbug_outs("\n\r");
    }

    rtx_dbug_outs("\n\r ****DONE INIT PCB STACK**** \n\r");
    sleep(3);
#endif

    // DO NOT CHANGE THIS!!!!
    current_process = NULL;
    // Start the OS

    // Initialize stack pointer!
    // asm("move.l 4(%a7),%a7");

    //TIMER0_TER = 2;
    asm( "move.w #0x2000,%sr" );

    currtime = 0; // Setting curr_time to reference start of OS

    current_primitive = RTX_INIT;
    asm("trap #6");
    
    return 0;
}
