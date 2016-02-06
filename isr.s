.intel_syntax noprefix

.extern interrupt_handler

.section .text
    .macro ISR_NOERRCODE intnum
        .globl isr\intnum

        isr\intnum:
            cli
            push 0
            push \intnum
            jmp interrupt_common
    .endm

    .macro ISR_ERRCODE intnum
        .globl isr\intnum

        isr\intnum:
            cli
            push \intnum
            jmp interrupt_common
    .endm

    interrupt_common:
        pusha
        call interrupt_handler
        popa
        add esp, 8
        sti
        iret

    ISR_NOERRCODE 0
    ISR_NOERRCODE 1
    ISR_NOERRCODE 2
    ISR_NOERRCODE 3
    ISR_NOERRCODE 4
    ISR_NOERRCODE 5
    ISR_NOERRCODE 6
    ISR_NOERRCODE 7
    ISR_ERRCODE 8
    ISR_NOERRCODE 9
    ISR_ERRCODE 10
    ISR_ERRCODE 11
    ISR_ERRCODE 12
    ISR_ERRCODE 13
    ISR_ERRCODE 14
    ISR_NOERRCODE 15
    ISR_NOERRCODE 16
    ISR_NOERRCODE 17
    ISR_NOERRCODE 18
    ISR_NOERRCODE 19
    ISR_NOERRCODE 20
    ISR_NOERRCODE 21
    ISR_NOERRCODE 22
    ISR_NOERRCODE 23
    ISR_NOERRCODE 24
    ISR_NOERRCODE 25
    ISR_NOERRCODE 26
    ISR_NOERRCODE 27
    ISR_NOERRCODE 28
    ISR_NOERRCODE 29
    ISR_NOERRCODE 30
    ISR_NOERRCODE 31
