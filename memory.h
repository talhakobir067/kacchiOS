#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void mm_init(uint32_t heap_size);
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif
