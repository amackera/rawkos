# the output executable is: main.s19

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS=-O0 -Wall -m5200 -pipe -nostdlib
DBGFLAG=-DDEBUG
DBIPROCFLAG=-DDEBUG_IPROC
DBPROCMANFLAG=-DDEBUG_PROCMAN
DBHOTKEYS=-DDEBUG_HOTKEYS
DBKCDFLAGS=-DDBKCD
DBSPCFLAGS=-DDBSPC
DBPROCPRIFLAGS=-DDBPROCPRI
DBCRT=-DDEBUG_CRT
DBIPCFLAGS=-DDBIPC
DBWC=-DDEBUG_WALLCLOCK
DBALL=$(DBGFLAG) $(DBIPROCFLAG) $(DBPROCMANFLAG) $(DBHOTKEYS) $(DBKCDFLAGS) $(DBSPCFLAGS) $(DBPROCPRIFLAGS) $(DBCRT) $(DBWC)
TIMEDISPLAY=-DDISPLAY_TIMER
VT100=
ASMFLAGS=-Wall -nostdlib -m5200 -Tmcf5307.ld -S
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
OBJDUMP=m68k-elf-objdump
ASM=start.s trap_entry.s timer_entry.s serial_entry.s
LDFLAGS = -Tmcf5307.ld -Wl,-Map=main.map

main-test: main.c
	$(CC) $(CFLAGS) $(DBIPROCFLAG) $(LDFLAGS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-test-time: main.c
	$(CC) $(CFLAGS) $(DBCRT) $(DBIPROCFLAG) $(DBHOTKEYS) $(TIMEDISPLAY) $(DBWC) $(VT100) $(LDFLAGS) \
	 -o main.o $(ASM) main.c outs.c \
	init.c const.c  envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
		pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-test-no-dbiproc: main.c
	$(CC) $(CFLAGS) $(DBGFLAG) $(DBHOTKEYS) $(DBPROCMANFLAG) $(LDFLAGS) \
	-o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-test-no-dbprocman: main.c
	$(CC) $(CFLAGS) $(DBGFLAG) $(DBHOTKEYS) $(DBIPROCFLAG) $(LDFLAGS) \
	-o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-test-no-db: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-test-only-hotkeys: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBHOTKEYS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-kcd-hotkeys: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBHOTKEYS) $(DBKCDFLAGS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-spc-hotkeys: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBHOTKEYS) $(DBPROCPRIFLAGS) $(DBSPCFLAGS) $(DBKCDFLAGS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-procpri-hotkeys: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBHOTKEYS) $(DBPROCPRIFLAGS) -o main.o $(ASM) main.c outs.c \
	init.c const.c 	envl_deque.c func.c envl.c pcb_pdeque.c  pcb.c \
	 	pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt

main-full: main.c
	$(CC) $(CFLAGS) $(DBHOTKEYS) $(LDFLAGS) -o main.o $(ASM) main.c init.c const.c \
	envl.c envl_deque.c pcb.c pcb_deque.c pcb_pdeque.c  \
	ipc.c mem.c func.c prim.c procpri.c crt.c nullproc.c procman.c \
	timer.c kcd.c scheduler.c timesrv.c spc.c uart.c func.c mem.c \
	wall_clock.c testproc.c 
	$(OBJCPY) --output-format=srec main.o main.s19

main-test-rtx: main.c
	$(CC) $(CFLAGS) $(DBHOTKEYS) $(LDFLAGS) -D_RTX_EVAL_ \
	-o main.o $(ASM) main.c outs.c rtx_eval1.o rtx_eval2.o rtx_eval3.o \
	rtx_eval4.o init.c const.c	envl_deque.c func.c envl.c pcb_pdeque.c \
	pcb.c pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
#	 rtxeval_dummy.c // since .o has this already (leads to multiple defintion error that no one can figure out)
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt









main-test-rtx-db: main.c
	$(CC) $(CFLAGS) $(DBALL) $(LDFLAGS) -D_RTX_EVAL_ \
	-o main.o $(ASM) main.c outs.c rtx_eval1.o rtx_eval2.o rtx_eval3.o \
	rtx_eval4.o init.c const.c	envl_deque.c func.c envl.c pcb_pdeque.c \
	pcb.c pcb_deque.c scheduler.c procman.c mem.c nullproc.c \
	swi.c ipc.c procpri.c timesrv.c uart.c wall_clock.c testproc.c \
	timer.c crt.c kcd.c spc.c
#	 rtxeval_dummy.c // since .o has this already (leads to multiple defintion error that no one can figure out)
	$(OBJCPY) --output-format=srec main.o main.s19
	$(OBJDUMP) -D main.o > list.txt


clean:
	cp start.s start.tmp
	rm -f *.s19 *.o *.map *.txt
	cp start.tmp start.s
	rm start.tmp
