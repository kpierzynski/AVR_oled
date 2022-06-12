#ifndef __AVR_OLED_H_
#define __AVR_OLED_H_

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "i2c.h"

#define OLED_ADDRESS 0x78

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#if OLED_HEIGHT != 64 && OLED_HEIGHT != 32
	#error This library can work with 64 or 32 pixel height oled displays
#endif

#define OLED_BUF_SIZE (OLED_WIDTH*OLED_HEIGHT / 8)

#define OLED_REFRESH_MIN 0x80
#define OLED_REFRESH_MID 0xB0
#define OLED_REFRESH_MAX 0xF0

#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF

#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA

#define OLED_SETVCOMDETECT 0xDB

#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9

#define OLED_SETMULTIPLEX 0xA8

#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10

#define OLED_SETSTARTLINE 0x40

#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22

#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8

#define OLED_SEGREMAP 0xA0

#define OLED_CHARGEPUMP 0x8D

#define OLED_EXTERNALVCC 0x1
#define OLED_SWITCHCAPVCC 0x2

#define OLED_ACTIVATE_SCROLL 0x2F
#define OLED_DEACTIVATE_SCROLL 0x2E
#define OLED_SET_VERTICAL_SCROLL_AREA 0xA3
#define OLED_RIGHT_HORIZONTAL_SCROLL 0x26
#define OLED_LEFT_HORIZONTAL_SCROLL 0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

void oled_init();
void oled_show();

void oled_pixel(int16_t x, int16_t y, uint8_t on);
void oled_char(int16_t x, int16_t y, char c, uint8_t on);
void oled_hex(int16_t x, int16_t y, uint8_t byte, uint8_t on);
void oled_string(int16_t x, int16_t y, const char *str, uint8_t on);
void oled_fill(uint8_t byte);
void oled_cls();
void oled_invert(uint8_t invert);
void oled_rect(int8_t x, int8_t y, uint8_t w, uint8_t h, uint8_t fill, uint8_t on);
void oled_text_area(int8_t x, int8_t y, uint8_t w, uint8_t h, const char *string, uint8_t on);
uint8_t oled_button(int8_t x, int8_t y, const char *string, uint8_t select);
void oled_arrow(int8_t x, int8_t y, uint8_t up, uint8_t on);

#endif
