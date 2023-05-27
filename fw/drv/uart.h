

//#define UART_BASE 0x00200000 // temp hardware

#define UART_BASE 0x1ffffff8

#define UART_RXB (0x00)
#define UART_LSR (0x04)

#define UART_LSR_RXR 0x01
#define UART_LSR_TXR 0x02

int uart_read_ready(void);
char uart_read_async(void);
char uart_read(void);

