#ifndef LIB_H
#define LIB_H

#include <stddef.h>

void *k_memcpy(void *dest, const void *src, size_t num);
void *k_memset(void *dest, uint8_t val, size_t num);

void kprintf(const char *fmt, ...);

#endif
