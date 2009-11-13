        .globl save_registers
        .even
save_registers:
        link.w  %a6, #0

        move.l  -4(%a7), -(%a7)         
        move.l  %a5, -(%a7)
        move.l  %a4, -(%a7)
        move.l  %a3, -(%a7)
        move.l  %a2, -(%a7)
        move.l  %a1, -(%a7)
        move.l  %a0, -(%a7)        

        move.l  %d0, -(%a7)
        move.l  %d1, -(%a7)
        move.l  %d2, -(%a7)
        move.l  %d3, -(%a7)
        move.l  %d4, -(%a7)
        move.l  %d5, -(%a7)
        move.l  %d6, -(%a7)
        move.l  %d7, -(%a7)
  
        unlk %a6
        rts