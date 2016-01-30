#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "myos.h"

void kernel_main() {
  vga_putchar('a', COLOR_RED, COLOR_BLACK);
  vga_putchar('a', COLOR_RED, COLOR_BLACK);
  vga_putchar('a', COLOR_RED, COLOR_BLACK);
  vga_putchar('a', COLOR_RED, COLOR_BLACK);
  vga_putchar('a', COLOR_RED, COLOR_BLACK);
}
