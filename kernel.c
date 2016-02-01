#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "myos.h"
#include "descriptor_table.h"

void kernel_main() {
    int i = 0;

    //init_descriptor_tables();

    uint8_t *msg = "This doesn't do anything yet.\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-------------x-----x---------]\n"
                   "[------------x-xxxxx----------]\n"
                   "[------------xxx--------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "[-----------------------------]\n"
                   "Copyright 2016 Tyler Jarjoura\n\n\n";

    vga_putstring(msg);

    while (1)
        ;

}
