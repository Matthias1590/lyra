#include <stdint.h>

#define VGA_TEXT_BUFFER ((volatile char*)0xB7F00)
#define VGA_ROW_COUNT 25
#define VGA_COL_COUNT 80

typedef enum {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    LIGHT_GRAY = 0x7,
    DARK_GRAY = 0x8,
    LIGHT_BLUE = 0x9,
    LIGHT_GREEN = 0xA,
    LIGHT_CYAN = 0xB,
    LIGHT_RED = 0xC,
    LIGHT_MAGENTA = 0xD,
    YELLOW = 0xE,
    WHITE = 0xF
} color_t;

void init_keyboard();
uint8_t get_scancode();
void init_pit();
void outb(uint16_t port, uint8_t val);
void wait_milliseconds(unsigned int milliseconds);
void clear_screen();
void write_char(char character, int row, int col, color_t color);
void write_string(const char* string, int row, int col, color_t color);
void set_cursor_position(int x, int y);
void handle_command();

uint8_t inb(uint16_t port);

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

void _start() {
    init_pit();
    init_keyboard();

    clear_screen();

    write_char('>', 0, 0, 0xf);

    set_cursor_position(2, 0);

    while (1) {
        uint8_t code = get_scancode();

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
        write_string(buffer, 0, 2, 0xf);

        set_cursor_position(2 + buffer_size, 0);
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

    clear_screen();
    write_char('>', 0, 0, 0xf);
    set_cursor_position(2, 0);

    if (streq(buffer, "scancode")) {
        get_scancode();

        write_string("waiting for key", 1, 0, 0xf);

        uint8_t sc = get_scancode();

        write_string("               ", 1, 0, 0xf);
        write_char(hex_char(sc >> 4), 1, 0, 0xf);
        write_char(hex_char(sc & 0x0F), 1, 1, 0xf);
    } else {
        write_string("unknown command '", 1, 0, 0xf);
        write_string(buffer, 1, 17, 0xf);
        write_char('\'', 1, 17 + buffer_size, 0xf);
    }
}

// Define VGA registers
#define VGA_CTRL_REG 0x3D4
#define VGA_DATA_REG 0x3D5

// Function to set the cursor position
void set_cursor_position(int x, int y) {
    unsigned short position = y * 80 + x;

    // Send high byte of the position
    outb(VGA_CTRL_REG, 14); // Send high byte of the position to VGA_CTRL_REG
    outb(VGA_DATA_REG, position >> 8);

    // Send low byte of the position
    outb(VGA_CTRL_REG, 15); // Send low byte of the position to VGA_CTRL_REG
    outb(VGA_DATA_REG, position);
}

// PIT I/O ports
#define PIT_COMMAND_PORT    0x43
#define PIT_DATA_PORT_CH0   0x40

// Function to initialize PIT for frequency of 1000 Hz (1 millisecond)
void init_pit() {
    uint16_t divisor = 1193; // 1193180 Hz / 1000 Hz = 1193
    outb(PIT_COMMAND_PORT, 0x36); // Set command byte
    outb(PIT_DATA_PORT_CH0, divisor & 0xFF); // Set low byte of divisor
    outb(PIT_DATA_PORT_CH0, divisor >> 8);   // Set high byte of divisor
}

// Function to wait for a specified number of milliseconds
void wait_milliseconds(unsigned int milliseconds) {
    for (unsigned int i = 0; i < milliseconds; ++i) {
        // Wait until the timer counts down to 0
        while (!(inb(0x61) & 0x20)) {} // Wait for timer 0's state to change
        outb(0x61, inb(0x61) & ~0x20); // Clear timer 0's output
    }
}

// Helper functions for I/O operations
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void clear_screen() {
    for (int row = 0; row < VGA_ROW_COUNT; row++) {
        for (int col = 0; col < VGA_COL_COUNT; col++) {
            write_char(' ', row, col, WHITE);
        }
    }
}

void write_char(char character, int row, int col, color_t color) {
    int offset = (row * 80 + col) * 2;

    VGA_TEXT_BUFFER[offset] = character;
    VGA_TEXT_BUFFER[offset + 1] = color;
}

void write_string(const char* string, int row, int col, color_t color) {
    int offset = (row * 80 + col) * 2;

    while (*string != '\0') {
        if (*string == '\n') {
            row++;
            offset = (row * 80) * 2;
        } else {
            VGA_TEXT_BUFFER[offset++] = *string;
            VGA_TEXT_BUFFER[offset++] = color;
        }
        string++;
    }
}

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

void init_keyboard() {
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

uint8_t get_scancode() {
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {} // Wait until the output buffer is full
    return inb(PS2_DATA_PORT); // Read the scancode
}
