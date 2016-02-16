/* vgatext.c
 *
 * This is a driver for the VGA text mode buffer provided by BIOS.
 */

#include <stdint.h>
#include <stddef.h>

#include "common.h"
#include "lib.h"
#include "sysio.h"

#define WIDTH  80
#define HEIGHT 25

static uint16_t *vga_text_buffer = (uint16_t *) 0xB8000;
static uint8_t color = 0x09; /* 0000 1111 --  White on black background */
static int row = 0, column = 0;

static void scroll() {
    k_memcpy(vga_text_buffer, vga_text_buffer + WIDTH, (HEIGHT-1) * WIDTH*(sizeof(uint16_t)));
    k_memset(vga_text_buffer + (HEIGHT-1) * WIDTH, 0x00, WIDTH*sizeof(uint16_t));
}

static void set_cursor(uint16_t pos) {
    outb(0x3D4, 14); /* Tell VGA controller we are setting high byte */
    outb(0x3D5, pos >> 8);

    outb(0x3D4, 15); /* And the low byte */
    outb(0x3D5, pos);
}

void vga_setcolor(enum vga_color foreground, enum vga_color background) {
    color = ((background & 0x7) << 4) | foreground;
}

void vga_putchar(uint8_t c) {
    uint16_t character = (color << 8) | c;
    int cursor = row * WIDTH + column;

    if ((c != '\n') && (c != '\t'))
        vga_text_buffer[cursor] = character;

    if (c == '\t')
        while ((++column % 8) != 0) ;
    else if (++column >= WIDTH || (c == '\n')) {
        column = 0;
        if (++row >= HEIGHT) {
            scroll();
            --row;
        }
    }

    set_cursor(row * WIDTH + column);
}

