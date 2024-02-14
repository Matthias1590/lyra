#include "drivers/ps2.h"
#include "asm.h"
#include <stdbool.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

void PS2_InitKeyboard(void) {
    outb(PS2_COMMAND_PORT, 0xAD); // Disable first PS/2 port
    outb(PS2_COMMAND_PORT, 0xA7); // Disable second PS/2 port
    inb(PS2_DATA_PORT); // Flush the output buffer
    outb(PS2_COMMAND_PORT, 0x20); // Read the current state of the controller
    uint8_t status = inb(PS2_DATA_PORT); // Get the current state of the controller
    status |= 0x1; // Set the first bit to 1
    status &= ~(0x10); // Set the fifth bit to 0
    outb(PS2_COMMAND_PORT, 0x60); // Set the current state of the controller
    outb(PS2_DATA_PORT, status); // Set the current state of the controller
    outb(PS2_COMMAND_PORT, 0xAE); // Enable first PS/2 port
    outb(PS2_COMMAND_PORT, 0xA8); // Enable second PS/2 port
    outb(PS2_DATA_PORT, 0xF4); // Enable keyboard
}

uint8_t PS2_GetScancode(void) {
    while (!PS2_HasScancode())
        ;

    return inb(PS2_DATA_PORT);
}

bool PS2_HasScancode(void) {
    return inb(PS2_STATUS_PORT) & 0x1;
}
