#include "uart.h"

#define Reg(reg) ((volatile unsigned char *)(UART_BASE + reg))
#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

int uart_read_ready(void) {
    return (ReadReg(UART_LSR) & UART_LSR_RXR) != 0;
}

char uart_read_async(void) {
    return ReadRegsssrrr(UART_RXB);
}

char uart_read(void) {
    while (!uart_read_ready());
    return uart_read_async();
}