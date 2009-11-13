#ifndef MEM_H
#define MEM_H

#include "globalvar.h"
#include "func.h"
#include "envl_deque.h"
#include "scheduler.h"

void* request_memory_block();
void* request_memory_block_handler();
int release_memory_block(void* MemoryBlock);
int release_memory_block_handler(void* MemoryBlock);

#endif /* MEM_H */
