#include "drv/gpio.h"
#include <stdio.h>
#include <stdint.h>

void lcd_init();
void lcd_puts(const uint32_t* s);

volatile int x;
volatile int y;

uint32_t* itoa32(int value, uint32_t* result, int base);

int main(void) {
        lcd_init();

/*
        x = 45;
        y = 5457;
        x = y * x;
        y = x / (y*345);
*/

        uint32_t buf[20] = U"prints";

        lcd_puts(buf);
        itoa32(420, buf, 10);
        lcd_puts(buf);

        return 0;
}

uint32_t* itoa32(int value, uint32_t* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = U'\0'; return result; }

    uint32_t* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = U"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = U'-';
    *ptr-- = U'\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

