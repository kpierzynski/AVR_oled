#include "oled.h"
#include "oled_font.h"

static uint8_t oled_buf[OLED_BUF_SIZE];
static const char symbols[] PROGMEM = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void oled_send(uint8_t byte, uint8_t control);
inline static void oled_send_cmd(uint8_t cmd);

void oled_init()
{
        oled_send_cmd(OLED_DISPLAYOFF);
        oled_send_cmd(OLED_SETDISPLAYCLOCKDIV);
        oled_send_cmd(OLED_REFRESH_MID);

        oled_send_cmd(OLED_SETMULTIPLEX);
#if OLED_HEIGHT == 64
        oled_send_cmd(0x3F);
#else
        oled_send_cmd(0x1F);
#endif
        oled_send_cmd(OLED_SETDISPLAYOFFSET);
        oled_send_cmd(0x00);
        oled_send_cmd(OLED_SETSTARTLINE | 0x0);
        oled_send_cmd(OLED_CHARGEPUMP);

        oled_send_cmd(0x14); // set to internal vcc?

        oled_send_cmd(OLED_MEMORYMODE);
        oled_send_cmd(0x00);
        oled_send_cmd(OLED_SEGREMAP | 0x1);
        oled_send_cmd(OLED_COMSCANDEC);

        oled_send_cmd(OLED_SETCOMPINS);
#if OLED_HEIGHT == 64
        oled_send_cmd(0x12);
#else
        oled_send_cmd(0x02);
#endif

        oled_send_cmd(OLED_SETCONTRAST);
        oled_send_cmd(0x8F);
        oled_send_cmd(OLED_SETPRECHARGE);
        oled_send_cmd(0xF1);
        oled_send_cmd(OLED_SETVCOMDETECT);
        oled_send_cmd(0x40);

        oled_send_cmd(OLED_DISPLAYALLON_RESUME);
        oled_send_cmd(OLED_NORMALDISPLAY);

        oled_send_cmd(OLED_DISPLAYON);
}

void oled_show()
{

        oled_send_cmd(OLED_COLUMNADDR);
        oled_send_cmd(0);
        oled_send_cmd(127);

        oled_send_cmd(OLED_PAGEADDR);
        oled_send_cmd(0);
#if OLED_HEIGHT == 64
        oled_send_cmd(7);
#else
        oled_send_cmd(3);
#endif

        i2c_start();
        i2c_write(OLED_ADDRESS);
        i2c_write(0x40);
        for (uint16_t i = 0; i < OLED_BUF_SIZE; i++)
                i2c_write(oled_buf[i]);
        i2c_stop();
}

static void oled_send(uint8_t byte, uint8_t control)
{
        i2c_start();
        i2c_write(OLED_ADDRESS);
        i2c_write(control);
        i2c_write(byte);
        i2c_stop();
}

inline static void oled_send_cmd(uint8_t cmd)
{
        oled_send(cmd, 0x00);
}

void oled_pixel(int16_t x, int16_t y, uint8_t on)
{
        x = OLED_WIDTH - x - 1;
        y = OLED_HEIGHT - y - 1;
        if ((x < 0) || (x >= OLED_WIDTH) || (y < 0) || (y >= OLED_HEIGHT))
                return;

        if (on)
                oled_buf[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7));
        else
                oled_buf[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7));
}

void oled_char(int16_t x, int16_t y, char c, uint8_t on)
{
        register uint8_t _x, _y;

        for (_x = 0; _x < 5; _x++)
                for (_y = 0; _y < 7; _y++)
                        if (pgm_read_byte(&simple_font[c - 32][_x]) & (1 << _y))
                                oled_pixel(x + _x, y + _y, on);
}

void oled_hex(int16_t x, int16_t y, uint8_t byte, uint8_t on)
{
        oled_char(x, y, pgm_read_byte(&symbols[(byte >> 4) & 0x0F]), on);
        oled_char(x + 6, y, pgm_read_byte(&symbols[(byte >> 0) & 0x0F]), on);
}

void oled_string(int16_t x, int16_t y, const char *str, uint8_t on)
{
        register uint8_t off_x = 0;
        register char c;

        while ((c = *str++))
        {
                oled_char(x + 6 * (off_x++), y, c, on);
        }
}

void oled_fill(uint8_t byte)
{
        register uint16_t i;
        for (i = 0; i < OLED_BUF_SIZE; i++)
                oled_buf[i] = byte;
}

inline void oled_cls()
{
        oled_fill(0);
}

void oled_invert(uint8_t invert)
{
        if (invert)
                oled_send_cmd(OLED_INVERTDISPLAY);
        else
                oled_send_cmd(OLED_NORMALDISPLAY);
}

void oled_rect(int8_t x, int8_t y, uint8_t w, uint8_t h, uint8_t fill, uint8_t on)
{
        for (uint8_t k = 0; k < h; k++)
        {
                for (uint8_t i = 0; i < w; i++)
                {
                        if (k == 0 || k == h - 1 || i == 0 || i == w - 1 || fill)
                                oled_pixel(x + i, y + k, on);
                }
        }
}

void oled_text_area(int8_t x, int8_t y, uint8_t w, uint8_t h, const char *string, uint8_t on)
{
        uint8_t len = strlen(string);

        oled_rect(x, y, w, h, 0, on);
        uint8_t offset = (w - 6 * len) / 2;
        oled_string(x + offset, y + 2, string, on);
}

uint8_t oled_button(int8_t x, int8_t y, const char *string, uint8_t select)
{
        uint8_t len = strlen(string);
        uint8_t w = 6*len+12;

        if (!select)
        {
                oled_text_area(x, y, w, 11, string, 1);
        }
        else
        {
                oled_rect(x + 1, y + 1, w-1, 10, 1, 1);
                oled_text_area(x, y, w, 11, string, 0);
        }

        return w;
}

void oled_arrow(int8_t x, int8_t y, uint8_t up, uint8_t on)
{
        for (uint8_t i = 0; i < 4; i++)
        {
                uint8_t y_lim = (up) ? 2 * i + 1 : -2 * i + 7;

                for (int8_t _y = 0; _y < y_lim; _y++)
                {
                        uint8_t x_pos = (up) ? 3 - i + _y : i + _y;
                        oled_pixel(x_pos + x, i + y, on);
                }
        }
}
