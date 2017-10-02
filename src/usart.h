#include <stdint.h>

// Common functions for USART2
int usart2_print(const char* str);
int init_usart2(uint32_t baudrate);
uint16_t usart2_getchar (void);
int usart2_putchar (uint16_t c);
