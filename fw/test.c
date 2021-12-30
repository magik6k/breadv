#include "drv/gpio.h"
#include "drv/shitty_shift.h"

int main(void) {
        gpio_set_bank0(0xf0);

        gpio_set_bank0(shiftRight4(0x30));
        gpio_set_bank0(shiftRight4(0x60));
        return 0;
}
