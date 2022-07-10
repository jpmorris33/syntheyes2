#include "PanelDriver.hpp"

void PanelDriver::init() {}
void PanelDriver::draw() {}
void PanelDriver::update(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::update_patterned(PanelBitmap bitmap, int offset) {}
void PanelDriver::update_nomirror(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::overdub(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::set_pattern(unsigned char pattern[16][16]) {}

//
//	Flip an RGB framebuffer 180 degrees - can be overridden for other formats, e.g. RGBA or 4-byte alignment with padding
//

void PanelDriver::rotate180(unsigned char *buffer, int w, int h) {
	unsigned char *startptr = buffer;
	unsigned char *endptr = buffer + (w * h * 3);
	unsigned char r,g,b;

	int pixels=w*h;
	pixels >>= 1;	// Halve the total since we're doing both ends at once

	for(int ctr=0;ctr<pixels;ctr++) {
		r=startptr[0];
		g=startptr[1];
		b=startptr[2];
		endptr -= 3;
		startptr[0]=endptr[0];
		startptr[1]=endptr[1];
		startptr[2]=endptr[2];
		endptr[0]=r;
		endptr[1]=g;
		endptr[2]=b;
		startptr += 3;
	}
}