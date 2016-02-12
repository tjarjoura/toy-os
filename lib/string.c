#include <stddef.h>
#include <stdint.h>

void *k_memcpy(void *dest, const void *src, size_t num) {
    size_t i;

    for (i = 0; i < num; i++)
        *(((uint8_t *) dest) + i) = *(((uint8_t *) src) + i);

    return dest;
}

void *k_memset(void *dest, uint8_t val, size_t num) {
    size_t i;

    for (i = 0; i < num; i++)
        *(((uint8_t *) dest) + i) = val;

    return dest;
}

