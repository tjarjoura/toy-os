/* vgatext.c
 *
 * This is a driver for the VGA text mode buffer provided by BIOS.
 */

#include <stdint.h>
#include <stddef.h>

#include "myos.h"

#define WIDTH  80
#define HEIGHT 25

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

static uint16_t *vga_text_buffer = (uint16_t *) 0xB8000;
static uint8_t color = 0x09; /* 0000 1111 --  White on black background */
static int row = 0, column = 0;

static void scroll() {
    k_memcpy(vga_text_buffer, vga_text_buffer + WIDTH, (HEIGHT-1) * WIDTH*(sizeof(uint16_t)));
    k_memset(vga_text_buffer + (HEIGHT-1) * WIDTH, 0x00, WIDTH*sizeof(uint16_t));
}

void vga_setcolor(enum vga_color foreground, enum vga_color background) {
    color = ((background & 0x7) << 4) | foreground;
}


void vga_putchar(uint8_t c) {
    uint16_t character = (color << 8) | c;
    int cursor = (row * WIDTH) + column;

    if (c != '\n')
        vga_text_buffer[cursor] = character;

    if (++column >= WIDTH || (c == '\n')) {
        column = 0;
        if (++row >= HEIGHT) {
            scroll();
            --row;
        }
    }
}

void vga_putstring(const uint8_t *str) {
    int i;

    for (i = 0; str[i] != '\0'; i++)
        vga_putchar(str[i]);
}

