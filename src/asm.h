#pragma once

#include <stdint.h>

/**
 * @brief Read a byte from a port.
 * 
 * @param port The port to read from.
 * 
 * @return The byte read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t data;

    asm volatile(
        "inb %1, %0"
        : "=a"(data)
        : "Nd"(port)
    );

    return data;
}

/**
 * @brief Write a byte to a port.
 * 
 * @param port The port to write to.
 * @param data The byte to write to the port.
 */
static inline void outb(uint16_t port, uint8_t data) {
    asm volatile(
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}
