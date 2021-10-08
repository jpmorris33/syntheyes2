#include "inttype.h"
class NeopixelDriver {

	public:
		virtual void init(int pin, int pixels);
		virtual void draw();
		virtual void setpixel(unsigned int pixel, uint8_t r, uint8_t g, uint8_t b, uint8_t intensity);
};
