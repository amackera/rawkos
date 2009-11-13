/*

  This is the main file of RAWKOS.

 */

#include "globalvar.h"
#include "init.h"

// Required for gcc not to go ballistic
int __main(void) {
  return 0;
}

int main(void) {
  //Disable the Interrupts
  asm("move.w #0x2700,%SR");
  return init();
}
