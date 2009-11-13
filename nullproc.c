#include "nullproc.h"

void null_process() {
    int i = 0;
    while(1) {
        i++;
#ifdef DEBUG
        rtx_dbug_outs("[NULL_PROCESS] null_process has run ");
        rtx_dbug_out_int10(i);
        rtx_dbug_outs(" times\n\r"); 
#endif
        release_processor();
    }
}
