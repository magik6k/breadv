#include "gpio.h"
#include "shitty_shift.h"

#define LCD_E  0x20
#define LCD_RS 0x10

#define CMD 0
#define CHR LCD_RS

// parts borrowed from http://www.embeddeddev.pl/obsluga-lcd-hd44780-komunikacja-jednokierunkowa/

#define LCD_CLEAR					0x01
#define LCD_HOME					0x02
#define LCDC_ENTRY_MODE				0x04
	#define LCD_EM_SHIFT_CURSOR		    0x00
	#define LCD_EM_SHIFT_DISPLAY	 	0x01
	#define LCD_EM_LEFT		   			0x00
	#define LCD_EM_RIGHT				0x02
#define LCD_ONOFF					0x08
	#define LCD_DISP_ON				    0x04
	#define LCD_CURSOR_ON				0x02
	#define LCDC_CURSOR_OFF				0x00
	#define LCDC_BLINK_ON				0x01
	#define LCDC_BLINK_OFF				0x00
#define LCD_SHIFT					0x10
	#define LCDC_SHIFT_DISP				0x08
	#define LCDC_SHIFT_CURSOR			0x00
	#define LCDC_SHIFT_RIGHT			0x04
	#define LCDC_SHIFT_LEFT				0x00
#define LCD_FUNC					0x20
	#define LCD_8_BIT					0x10
	#define LCD_4_BIT					0x00
	#define LCDC_TWO_LINE				0x08
	#define LCDC_FONT_5x10				0x04
	#define LCDC_FONT_5x7				0x00
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80

static void send_half(int d, int sys) {
    gpio_set_bank0(d | sys);
    gpio_set_bank0(d | sys | LCD_E);
    gpio_set_bank0(d | sys);
}

static void send_byte(int d, int sys) {
    send_half(shiftRight4(d & 0xf0), sys);
    send_half(d & 0x0f, sys);
}

void lcd_init() {
    gpio_set_bank0(0);

    send_half(0x03, CMD);
    send_half(0x03, CMD);
    send_half(0x03, CMD);
    send_half(0x02, CMD);

    send_byte(LCD_FUNC | LCD_4_BIT | LCDC_TWO_LINE | LCDC_FONT_5x10, CMD);
    send_byte(LCD_ONOFF | LCD_DISP_ON, CMD);
    send_byte(LCD_CLEAR, CMD);
    //send_byte(LCDC_ENTRY_MODE | LCD_EM_SHIFT_CURSOR | LCD_EM_RIGHT, CMD);

    send_byte('B', CHR);
    send_byte('r', CHR);
    send_byte('e', CHR);
    send_byte('a', CHR);
    send_byte('d', CHR);
    send_byte('V', CHR);
}

