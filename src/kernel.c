#include <stdint.h>
#include "asm.h"
#include "drivers/pit.h"
#include "drivers/vga.h"
#include "drivers/ps2.h"

void handle_command();

char scancodes[] = {
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1e] = 'a',
    [0x1f] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x2c] = 'z',
    [0x2d] = 'x',
    [0x2e] = 'c',
    [0x2f] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x39] = ' ',
    [0x1c] = '\n',
    [0x0e] = '\b',
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0a] = '9',
    [0x0b] = '0',
    [0x0c] = '-',
    [0x0d] = '=',
    [0x1a] = '[',
    [0x1b] = ']',
    [0x29] = '`',
    [0x27] = ';',
    [0x2b] = '\\',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
};

char buffer[64] = {' '};
int buffer_size = 0;

void __attribute__((section(".start"))) _start(void) {
    PIT_Init();
    PS2_InitKeyboard();

    VGA_ClearScreen();

    VGA_WriteChar('>', 0, 0, 0xf);

    VGA_SetCursorPos(0, 2);

    while (1) {
        uint8_t code = PS2_GetScancode();

        char c = scancodes[code];

        if (c == '\n') {
            handle_command();

            for (int i = 0; i < sizeof(buffer); i++) buffer[i] = ' ';
            buffer_size = 0;

            continue;
        } else if (c == '\b') {
            if (buffer_size > 0) {
                buffer[--buffer_size] = ' ';
            }
        } else if (c != 0 && buffer_size < sizeof(buffer) - 1) {
            buffer[buffer_size++] = c;
        }

        buffer[63] = '\0';

        // write_char(buffer[0], 0, 0, 0xf);
        VGA_WriteString(buffer, 0, 2, 0xf);

        VGA_SetCursorPos(0, 2 + buffer_size);
    }
}

char hex_char(uint8_t n) {
    if (n < 10) return '0' + n;
    return 'A' + n - 10;
}

int streq(char* a, char* b) {
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) return 0;
        a++;
        b++;
    }

    return *a == *b;
}

void handle_command() {
    buffer[buffer_size] = '\0';

    VGA_ClearScreen();
    VGA_WriteChar('>', 0, 0, 0xf);
    VGA_SetCursorPos(0, 2);

    if (streq(buffer, "scancode")) {
        PS2_GetScancode();

        VGA_WriteString("waiting for key", 1, 0, 0xf);

        uint8_t sc = PS2_GetScancode();

        VGA_WriteString("               ", 1, 0, 0xf);
        VGA_WriteChar(hex_char(sc >> 4), 1, 0, 0xf);
        VGA_WriteChar(hex_char(sc & 0x0F), 1, 1, 0xf);
    } else if (streq(buffer, "wait")) {
        VGA_WriteString("waiting for 1 second", 1, 0, 0xf);
        PIT_WaitMillis(1000);
        VGA_WriteString("done                ", 1, 0, 0xf);
    } else {
        VGA_WriteString("unknown command '", 1, 0, 0xf);
        VGA_WriteString(buffer, 1, 17, 0xf);
        VGA_WriteChar('\'', 1, 17 + buffer_size, 0xf);
    }
}
