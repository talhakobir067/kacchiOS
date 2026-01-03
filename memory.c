#include "memory.h"

typedef struct mem_block {
    uint32_t size;
    uint8_t free;
    struct mem_block* next;
} mem_block_t;

#define BLOCK_SIZE sizeof(mem_block_t)

static mem_block_t* heap_head;

void mm_init(uint32_t heap_size) {
    heap_head = (mem_block_t*)0x100000;
    heap_head->size = heap_size - BLOCK_SIZE;
    heap_head->free = 1;
    heap_head->next = 0;
}

void* kmalloc(uint32_t size) {
    mem_block_t* curr = heap_head;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + BLOCK_SIZE) {
                mem_block_t* split =
                    (mem_block_t*)((uint8_t*)curr + BLOCK_SIZE + size);

                split->size = curr->size - size - BLOCK_SIZE;
                split->free = 1;
                split->next = curr->next;

                curr->next = split;
                curr->size = size;
            }
            curr->free = 0;
            return (void*)((uint8_t*)curr + BLOCK_SIZE);
        }
        curr = curr->next;
    }
    return 0;
}

void kfree(void* ptr) {
    if (!ptr) return;
    mem_block_t* block =
        (mem_block_t*)((uint8_t*)ptr - BLOCK_SIZE);
    block->free = 1;
}
