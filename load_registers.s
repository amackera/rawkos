        .globl load_registers
        .even
load_registers:
        link.w  %a6, #0
        move.l  8(%a6), %a7     

        adda.l   #8, %a7        
        move.l  (%a7)+, %d7
        move.l  (%a7)+, %d6
        move.l  (%a7)+, %d5
        move.l  (%a7)+, %d4
        move.l  (%a7)+, %d3
        move.l  (%a7)+, %d2
        move.l  (%a7)+, %d1
        move.l  (%a7)+, %d0

        move.l  (%a7)+, %a0
        move.l  (%a7)+, %a1
        move.l  (%a7)+, %a2
        move.l  (%a7)+, %a3
        move.l  (%a7)+, %a4        
        move.l  (%a7)+, %a5
        move.l  (%a7)+, %a6

        unlk    %a6
        rts