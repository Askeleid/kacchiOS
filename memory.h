#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void memory_init(void);
void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif