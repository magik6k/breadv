#include<avr/io.h>
#include<avr/wdt.h>

#define D1_l1 (1<<PA0)
#define D1_l6 (1<<PA1)
#define D1_l2 (1<<PA2)
#define D1_l7 (1<<PA3)
#define D1_l3 (1<<PA4)
#define D1_l5 (1<<PA5)
#define D1_l8 (1<<PA6)
#define D1_l4 (1<<PA7)

#define D2_l7 (1<<PB0)
#define D2_l8 (1<<PB1)
#define D2_l5 (1<<PB2)
#define D2_l4 (1<<PB3)
#define D2_l6 (1<<PB4)
#define D2_l3 (1<<PB5)
#define D2_l1 (1<<PB6)
#define D2_l2 (1<<PB7)

#define D1_A D1_l1
#define D1_B D1_l2
#define D1_C D1_l3
#define D1_D D1_l4
#define D1_E D1_l5
#define D1_F D1_l6
#define D1_G D1_l7
#define D1_DP D1_l8

#define D2_A D2_l1
#define D2_B D2_l2
#define D2_C D2_l3
#define D2_D D2_l4
#define D2_E D2_l5
#define D2_F D2_l6
#define D2_G D2_l7
#define D2_DP D2_l8

const uint8_t d1lut[] = {
    (D1_A|D1_B|D1_C|D1_D|D1_E|D1_F), // 0
    (D1_E|D1_F), // 1
    (D1_A|D1_B|D1_D|D1_E|D1_G), // 2
    (D1_A|D1_B|D1_C|D1_D|D1_G), // 3
    (D1_B|D1_C|D1_F|D1_G), // 4
    (D1_A|D1_C|D1_D|D1_F|D1_G), // 5
    (D1_A|D1_C|D1_D|D1_E|D1_F|D1_G), // 6
    (D1_A|D1_B|D1_C), // 7
    (D1_A|D1_B|D1_C|D1_D|D1_E|D1_F|D1_G), // 8
    (D1_A|D1_B|D1_C|D1_D|D1_F|D1_G), // 9
    (D1_A|D1_B|D1_C|D1_E|D1_F|D1_G), // a
    (D1_C|D1_D|D1_E|D1_F|D1_G), // b
    (D1_A|D1_D|D1_E|D1_F), // c
    (D1_B|D1_C|D1_D|D1_E|D1_G), // d
    (D1_A|D1_D|D1_E|D1_F|D1_G), // e
    (D1_A|D1_E|D1_F|D1_G), // f
};

const uint8_t d2lut[] = {
    (D2_A|D2_B|D2_C|D2_D|D2_E|D2_F), // 0
    (D2_E|D2_F), // 1
    (D2_A|D2_B|D2_D|D2_E|D2_G), // 2
    (D2_A|D2_B|D2_C|D2_D|D2_G), // 3
    (D2_B|D2_C|D2_F|D2_G), // 4
    (D2_A|D2_C|D2_D|D2_F|D2_G), // 5
    (D2_A|D2_C|D2_D|D2_E|D2_F|D2_G), // 6
    (D2_A|D2_B|D2_C), // 7
    (D2_A|D2_B|D2_C|D2_D|D2_E|D2_F|D2_G), // 8
    (D2_A|D2_B|D2_C|D2_D|D2_F|D2_G), // 9
    (D2_A|D2_B|D2_C|D2_E|D2_F|D2_G), // a
    (D2_C|D2_D|D2_E|D2_F|D2_G), // b
    (D2_A|D2_D|D2_E|D2_F), // c
    (D2_B|D2_C|D2_D|D2_E|D2_G), // d
    (D2_A|D2_D|D2_E|D2_F|D2_G), // e
    (D2_A|D2_E|D2_F|D2_G), // f
};

#define getbit(i, src, dst) (((i&(1<<src))>>src)<<dst)

void main() {
    DDRB |= 0xff;
    DDRA |= 0xff;
    DDRC |= 0x0;

    PORTA = 0xff;
    PORTB = 0xff;

    while(1) {
            uint8_t val = PINC;

            PORTA = ~d1lut[getbit(val, 3, 0) | getbit(val, 2, 1) | getbit(val, 1, 2) | getbit(val, 0, 3)];
            PORTB = ~d2lut[getbit(val, 7, 0) | getbit(val, 6, 1) | getbit(val, 5, 2) | getbit(val, 4, 3)];
            wdt_reset();
    }
}