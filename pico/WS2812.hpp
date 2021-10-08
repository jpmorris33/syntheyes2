#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "../NeopixelDriver.hpp"

#define MAXPIXELS 256

class WS2812 : public NeopixelDriver {

	public:
		void init(int pin, int pixels);
		void draw();
		void setpixel(unsigned int pixel, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity);

	private:
		PIO pio;
		uint sm_ws2812;
		uint ws2812_offs;

		int pixelcount;
		uint32_t rgbbuf[MAXPIXELS];

};
