#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "common.h"

void init_idt();

/* This contains our saved state, pushed to the stack when an interrupt occurs */
struct saved_registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha */
    uint32_t int_num, err_code;
};

/* A void function pointer type, used as a callback for an interrupt number */
typedef void (*interrupt_handler_t) (struct saved_registers);

/* Register an interrupt handler */
void set_interrupt_handler(uint8_t int_num, interrupt_handler_t handler);

#endif
