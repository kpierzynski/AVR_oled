#include <avr/io.h>

#include "i2c.h"
#include "oled.h"

int main()
{
	i2c_init(100);
	oled_init();

	oled_pixel(127,63,1);

	uint8_t w = oled_button(1,2, "Ok", 1 );
	oled_button( w + 2, 2, "Cancel", 0 );

    oled_rect(0,15,128,22,0,1);

	oled_arrow(4,40,1,1);

	oled_show();

	while (1)
	{

	}

	return 0;
}
