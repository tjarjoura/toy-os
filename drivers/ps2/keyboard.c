#include "sysio.h"

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_ECHO 0xEE
#define PS2_TEST_FAILED 0xFC
#define PS2_TEST_PASSED 0xAA

#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT    0x60

char ps2_getchar() {
}
