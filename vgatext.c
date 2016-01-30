#include <stdint.h>

#include "myos.h"

#define WIDTH  80
#define HEIGHT 25

static uint16_t *vga_text_buffer = (uint16_t *) 0xB8000;
static int cursor = 0;

void vga_putchar(uint8_t c, enum vga_color foreground, enum vga_color background) {
    uint16_t character = ((((background & 0x7) << 4) | foreground) << 8) | c;
    vga_text_buffer[cursor++] = character;

    if (cursor > WIDTH*HEIGHT)
        cursor = 0;
}
