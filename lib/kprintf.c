#include <stdarg.h>
#include "myos.h"

static void write_int(int x, int base) {
    static const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char buffer[20];
    int i = 0;

    if (x == 0) {
        vga_putchar('0');
        return;
    }

    while (x != 0) {
        buffer[i++] = digits[x % base];
        x /= base;
    }

    for (i = i - 1; i >= 0; i--)
        vga_putchar(buffer[i]);
}

static void write_string(const char *s) {
    while (*s)
        vga_putchar(*s++);
}

void kprintf(const char *fmt, ...) {
    va_list ap;
    const char *p;

    va_start(ap, fmt);

    for (p = fmt; *p; p++) {
        if (*p != '%') {
            vga_putchar(*p);
            continue;
        }

        switch (*(++p)) {
            case '%':
                vga_putchar('%');
                continue;
            case 'd':
                write_int(va_arg(ap, int), 10);
                continue;
            case 'x':
                write_int(va_arg(ap, int), 16);
                continue;
            case 's':
                write_string(va_arg(ap, char *));
                continue;
            default:
                continue;
        }
    }
}
