#include "memory.h"

#define MEMORY_POOL_SIZE (64 * 1024)

static uint8_t memory_pool[MEMORY_POOL_SIZE] __attribute__((aligned(4)));


typedef struct memory_block {
    uint32_t size;
    uint8_t is_free;
    struct memory_block *next;
} memory_block_t;

static memory_block_t *free_list = NULL;

void memory_init(void) {
    memory_block_t *first = (memory_block_t *)memory_pool;

    first->size = MEMORY_POOL_SIZE - sizeof(memory_block_t);
    first->is_free = 1;
    first->next = NULL;

    free_list = first;
}

void *kmalloc(uint32_t size) {
    if (size == 0)
        return NULL;
    
    if (size > MEMORY_POOL_SIZE)
        return NULL;

    uint32_t aligned_size = (size + 3) & ~3;

    memory_block_t *current = free_list;

    while (current != NULL) {
        if (current->is_free == 1 && current->size >= aligned_size) {

            if (current->size - aligned_size >= sizeof(memory_block_t) + 4) {
                memory_block_t *new_block =
                    (memory_block_t *)((uint8_t *)(current + 1) + aligned_size);

                new_block->size = current->size - aligned_size - sizeof(memory_block_t);
                    
                new_block->is_free = 1;
                new_block->next = current->next;

                current->size = aligned_size;
                current->next = new_block;
            }

            current->is_free = 0;
            return (void *)(current + 1);
        }

        current = current->next;
    }

    return NULL;
}


void kfree(void *ptr) {
    if (ptr == NULL)
        return;

    memory_block_t *current = free_list;

    while (current != NULL) {
        if ((void *)(current + 1) == ptr) {

            if (current->is_free == 1)
                return;

            current->is_free = 1;
            break;
        }

        current = current->next;
    }

    if (current == NULL)
        return;

    current = free_list;

    while (current != NULL && current->next != NULL) {
        if (current->is_free == 1 && current->next->is_free == 1) {
            current->size += sizeof(memory_block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
