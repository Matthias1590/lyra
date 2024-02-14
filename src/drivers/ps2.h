#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the PS/2 keyboard.
 */
void PS2_InitKeyboard(void);

/**
 * @brief Gets the next scancode from the PS/2 keyboard.
 * 
 * @return The next scancode.
 */
uint8_t PS2_GetScancode(void);

/**
 * @brief Checks if there is a scancode available to read.
 * 
 * @return Whether there is a scancode available to read.
 */
bool PS2_HasScancode(void);
