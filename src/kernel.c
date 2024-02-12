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

void clear_screen();
void write_char(char character, int row, int col, color_t color);
void write_string(const char* string, int row, int col, color_t color);

uint8_t inb(uint16_t port);

void _start() {
    init_pit();

    while (1) {
        for (int i = 0; i < VGA_ROW_COUNT; i++) {
            clear_screen();
            
            write_string("Hello", i, 0, 0xf);

            wait_milliseconds(100000);
        }
    }

    while (1);
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
