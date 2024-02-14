#pragma once

#include <stdint.h>

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
} VGA_Color;

/**
 * @brief Clears the screen.
 */
void VGA_ClearScreen(void);

/**
 * @brief Writes a character to the screen.
 * 
 * @param character The character to write.
 * @param row The row to write the character to.
 * @param column The column to write the character to.
 * @param color The color to write the character in.
 */
void VGA_WriteChar(char character, uint8_t row, uint8_t column, VGA_Color color);

/**
 * @brief Writes a string to the screen.
 * 
 * @param string The string to write.
 * @param row The row to write the string to.
 * @param column The column to write the string to.
 * @param color The color to write the string in.
 */
void VGA_WriteString(const char* string, uint8_t row, uint8_t column, VGA_Color color);

/**
 * @brief Sets the cursor position.
 * 
 * @param row The row to set the cursor to.
 * @param column The column to set the cursor to.
 */
void VGA_SetCursorPos(uint8_t row, uint8_t column);
