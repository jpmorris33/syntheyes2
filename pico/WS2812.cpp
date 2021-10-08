//
//  Pico WS2812 driver
//

#include "WS2812.hpp"

#include "hardware/gpio.h"
#include "ws2812.pio.h"

static uint8_t scale[128][256];

void WS2812::init(int pin, int pixels) {

	pixelcount=0;
	if(pixels < 1 || pixels > MAXPIXELS) {
		return;
	}

	sm_ws2812=2; // 0,1 = HUB display
	pio = pio0;
	ws2812_offs = pio_add_program(pio, &ws2812_program);
	ws2812_program_init(pio, sm_ws2812, ws2812_offs, pin, 800000, false);

	pixelcount=pixels;

	// Build intensity lookup table
	for(int intensity=0;intensity<128;intensity++) {
		for(int ctr=0;ctr<256;ctr++) {
			scale[intensity][ctr]=(ctr * intensity)>>8;
			if(ctr > 0 && scale[intensity][ctr] < 1) {
				scale[intensity][ctr]=1;
			}
		}
	}

}

void WS2812::setpixel(unsigned int pixel, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity) {
	if(pixel < 0 || !pixelcount || pixel >= pixelcount) {
		return;
	}
	//rgbbuf[pixel]=0x80108000;
	intensity >>= 1; // scale to 128
	r=scale[intensity][r];
	g=scale[intensity][g];
	b=scale[intensity][b];
	rgbbuf[pixel]=(uint32_t)(r)<<16|(uint32_t)(g)<<24|(uint32_t)(b)<<8;
}

void WS2812::draw() {
	if(!pixelcount) {
		return;
	}
	uint32_t *ptr = &rgbbuf[0];
	for(int ctr=0;ctr<pixelcount;ctr++) {
		pio_sm_put_blocking(pio, sm_ws2812, *ptr++);
	}
}
