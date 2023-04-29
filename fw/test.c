#include "drv/gpio.h"
#include <stdio.h>
#include <stdint.h>

void lcd_init();
void lcd_puts(const char* s);

int uart_read_ready(void);
char uart_read_async(void);
char uart_read(void);

void lcd_shift_disp();
void lcd_shift_cur();

char* itoa(int value, char* result, int base);

int main(void) {
        lcd_init();

        char buf[2] = ">";

        lcd_puts(buf);
        //itoa(420, buf, 10);
        //lcd_puts(buf);

        while(1){
                char c = uart_read();

                switch(c) {
                case 'a':
                    lcd_shift_disp();
                    break;
                case 'b':
                    lcd_shift_cur();
                    break;
                default:
                    buf[0] = c;
                    buf[1] = '\0';
                    lcd_puts(buf);
                }
        }

        return 0;
}

char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

