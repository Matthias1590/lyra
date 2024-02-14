#include "drivers/vga.h"
#include "asm.h"

#define VGA_TEXT_BUFFER ((volatile char*)0xB7F00)

#define VGA_ROW_COUNT 25
#define VGA_COL_COUNT 80

#define VGA_CTRL_REG 0x3D4
#define VGA_DATA_REG 0x3D5

static inline uint16_t RowAndColumnToIndex(uint8_t row, uint8_t column) {
    return row * 80 + column;
}

static inline uint16_t RowAndColumnToOffset(uint8_t row, uint8_t column) {
    return RowAndColumnToIndex(row, column) * 2;
}

void VGA_SetCursorPos(uint8_t row, uint8_t column) {
    uint16_t index = RowAndColumnToIndex(row, column);

    outb(VGA_CTRL_REG, 14);
    outb(VGA_DATA_REG, index >> 8);

    outb(VGA_CTRL_REG, 15);
    outb(VGA_DATA_REG, index);
}

void VGA_ClearScreen(void) {
    for (uint8_t row = 0; row < VGA_ROW_COUNT; row++) {
        for (uint8_t column = 0; column < VGA_COL_COUNT; column++) {
            VGA_WriteChar(' ', row, column, WHITE);
        }
    }
}

void VGA_WriteChar(char character, uint8_t row, uint8_t column, VGA_Color color) {
    uint16_t offset = RowAndColumnToOffset(row, column);

    VGA_TEXT_BUFFER[offset] = character;
    VGA_TEXT_BUFFER[offset + 1] = color;
}

void VGA_WriteString(const char* string, uint8_t row, uint8_t col, VGA_Color color) {
    uint16_t offset = RowAndColumnToOffset(row, col);

    while (*string != '\0') {
        if (*string == '\n') {
            row++;
            offset = RowAndColumnToOffset(row, col);
        } else {
            VGA_TEXT_BUFFER[offset++] = *string;
            VGA_TEXT_BUFFER[offset++] = color;
        }
        string++;
    }
}
