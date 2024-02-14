#include "drivers/pit.h"
#include "asm.h"

#define PIT_COMMAND_PORT 0x43
#define PIT_DATA_PORT_CH0 0x40

void PIT_Init(void) {
    uint16_t divisor = 1193;
    outb(PIT_COMMAND_PORT, 0b00110000);
    outb(PIT_DATA_PORT_CH0, divisor & 0xFF);
    outb(PIT_DATA_PORT_CH0, divisor >> 8);
}

void PIT_WaitMillis(uint16_t milliseconds) {
    // todo
}
