#include "interrupt.h"
#include "lib.h"
#include "sysio.h"

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_ECHO 0xEE
#define PS2_TEST_FAILED 0xFC
#define PS2_TEST_PASSED 0xAA

#define PS2_DATA_PORT    0x60
#define PS2_COMMAND_PORT 0x64

#define KEYDOWN 1
#define KEYUP   2

struct key_event {
    char key;
    int type;
};

static uint8_t read_byte() {
    static const int timeout = 1000;
    int i;
    while (!(inb(PS2_COMMAND_PORT) & 0x1)) {
        if (i++ < timeout) {
            return 0xFF;
        }
    }

    return inb(PS2_DATA_PORT);
}

static void send_byte(uint8_t command, uint8_t port) {
    /* Wait for input buffer to clear */
    while (inb(PS2_COMMAND_PORT) & 0x2)
        ;
    outb(port, command);
}

static void send_to_device(int dev, uint8_t data) {
    if (dev == 1) {
      send_byte(0xD3, PS2_COMMAND_PORT); /* Tell controller we're talking to second device */
    }

    send_byte(data, PS2_DATA_PORT);
}

/* Callback for when the keyboard interrupts */
static void interrupt_cb(struct saved_registers saved_registers) {
    static int timeout = 1000;
    int i = 0, j = 0, n_scancodes;
    uint8_t scancodes[10];

    kprintf("[KEYBOARD INTERRUPT] ");
    for (i = 0; i < n_scancodes; i++)
        kprintf("%x", scancodes[i]);
    kprintf("\n");
}

void cpu_reset() {
    outb(PS2_COMMAND_PORT, 0xFE);
}

uint8_t ps2_poll_loop() {
    uint8_t scan_code;
    if (inb(PS2_COMMAND_PORT) & 0x1) {
        scan_code = inb(PS2_DATA_PORT);
        if (scan_code == 0x2D) /* Pressed 'r' -- Trigger CPU Reset */
            cpu_reset();

        return scan_code;
    }

    return 0;
}

static uint8_t get_configuration_byte() {
    send_byte(0x20, PS2_COMMAND_PORT);
    return read_byte();
}

static void set_configuration_byte(uint8_t configuration_byte) {
    send_byte(0x60, PS2_COMMAND_PORT);
    send_byte(configuration_byte, PS2_DATA_PORT);
}

static void set_irq(uint8_t irq_num) {
    uint16_t port = (irq_num < 8) ? 0x21 : 0xA1;
    uint8_t value;

    if (port == 0xA1)
        irq_num -= 8;

    value = inb(port) | (1 << irq_num);
    outb(port, value);
}

void init_ps2() {
    int dual_port = -1;
    uint8_t configuration_byte;

    /* Disable PS/2 ports during initialization */
    send_byte(0xAD, PS2_COMMAND_PORT);
    send_byte(0xA7, PS2_COMMAND_PORT);

    /* Flush PS/2 output buffer */
    read_byte();

    /* Get the configuration byte */
    configuration_byte = get_configuration_byte();

    /* If bit 5 isn't set we know our PS/2 Controller doesn't support 2 ports */
    if (!(configuration_byte & 0x10))
        dual_port = 0;

    /* Clear bits 0, 1, and 6 to disable interrupts and translation */
    configuration_byte &= 0xBC;

    /* Set the configuration byte */
    set_configuration_byte(configuration_byte);

    /* Test PS/2 Controller */
    send_byte(0xAA, PS2_COMMAND_PORT);
    if (read_byte() != 0x55) {
        kprintf("PS/2 Controller Failed Self Test\n");
        return;
    }

    /* Test if we have dual-port PS/2 support -- bit 5 shouldn't be set after enabling device 2 */
    if (dual_port == -1) {
        /* Enable 2nd port */
        send_byte(0xA8, PS2_COMMAND_PORT);

        configuration_byte = get_configuration_byte();
        dual_port = (configuration_byte & 0x10) ? 0 : 1;

        /* Disable 2nd port */
        send_byte(0xA7, PS2_COMMAND_PORT);
    }

    /* Test the PS/2 Ports */
    send_byte(0xAB, PS2_COMMAND_PORT);
    if (read_byte() != 0x00) {
        kprintf("PS/2 Port 1 Failed Test\n");
        return;
    }

    if (dual_port) {
        send_byte(0xA9, PS2_COMMAND_PORT);
        if (read_byte() != 0x00) {
            kprintf("PS/2 Port 2 Failed Test\n");
            return;
        }
    }

    /* Re-set configuration byte */
    configuration_byte = get_configuration_byte();
    configuration_byte |= 0x43;
    set_configuration_byte(configuration_byte);

    /* Re-enable ports */
    send_byte(0xAE, PS2_COMMAND_PORT);
    if (dual_port)
        send_byte(0xA8, PS2_COMMAND_PORT);


    /* Reset 1st device */
    send_byte(0xFF, PS2_DATA_PORT);
    /* Device should first send 0xFA (ACK) and then 0xAA for success */
    if ((read_byte() != 0xFA) || (read_byte() != 0xAA)) {
        kprintf("PS/2 Device 1 Failed Reset\n");
        return;
    }

    if (dual_port) {
        send_byte(0xD4, PS2_COMMAND_PORT);
        if ((read_byte() != 0xFA) || (read_byte() != 0xAA)) {
            kprintf("PS/2 Device 2 Failed Reset\n");
            return;
        }
    }

    set_interrupt_handler(0x21, interrupt_cb);
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
    asm("sti");

    kprintf("PS/2 Initialization Finished.\n");
}
