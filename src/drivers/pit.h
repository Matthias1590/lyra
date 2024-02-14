#pragma once

#include <stdint.h>

/**
 * @brief Initializes the PIT (Programmable Interval Timer).
 */
void PIT_Init(void);

/**
 * @brief Waits for a given number of milliseconds.
 * 
 * @param milliseconds The number of milliseconds to wait.
 */
void PIT_WaitMillis(uint16_t milliseconds);
