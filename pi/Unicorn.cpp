/**
 * Render to a Unicorn HD panel via SPI
 */

#include "Unicorn.hpp"

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

unsigned char spioutputbuf[769];
unsigned char lastbuf[769];
extern bool transmitter;

//
//	Init the Unicorn HD driver
//
void Unicorn::init() {
	wiringPiSetup();
	wiringPiSPISetup(0,9000000);
	memset(lastbuf,0x69,769); // Fill it with garbage
}

//
//	Put the framebuffer onto the Unicorn HD panel
//
void Unicorn::draw() {
	unsigned char *inptr = &framebuffer[0];
	unsigned char *outptr = &spioutputbuf[0];
	*outptr++=0x72;   // for addressing multiple panels, it's 0x73 + zero-based panel address

	int windowwidth = EYE_WIDTH * 3;	// 16 RGB triplets

	if(transmitter) {
		// Draw the right-hand half on panel 1
		for(int ctr=0;ctr<BITMAP_HEIGHT;ctr++)  {
			// Physical panel is only half the width of the virtual panel
			inptr += windowwidth;  			// Skip the first half of panel 1
			memcpy(outptr,inptr,windowwidth);	// Write the second half of panel 1
			inptr += windowwidth;			// Jockey past the block we've just written
			outptr += windowwidth;			// Jockey past the block we've just written

			inptr += windowwidth;			// Skip the first half of panel 2
			inptr += windowwidth; 			// Skip the second half of panel 2
		}
	} else {
		for(int ctr=0;ctr<BITMAP_HEIGHT;ctr++)  {
			// Physical panel is only half the width of the virtual panel
			inptr += windowwidth;			// Jockey past first half of panel 1
			inptr += windowwidth;			// Jockey past second half of panel 1

			memcpy(outptr,inptr,windowwidth);	// Write the first half of panel 2
			outptr += windowwidth;			// Jockey past the block we've just written
			inptr += windowwidth;			// Jockey past the block we've just written
			inptr += windowwidth; 			// Skip the second half of panel 2
		}
	}

	// Only do the SPI transfer if we actually need to 
	if(memcmp(lastbuf,spioutputbuf,769)) {
		memcpy(lastbuf,spioutputbuf,769);
		wiringPiSPIDataRW(0,spioutputbuf,769);
	}
}

#if 0
//
//	Put the framebuffer onto two Unicorn HD panels (experimental)
//
void Unicorn::draw() {
	unsigned char *inptr = &framebuffer[0];
	unsigned char *outptr = &spioutputbuf[0];
	*outptr++=0x72;   // for addressing multiple panels, it's 0x73 + zero-based panel address

	int windowwidth = EYE_WIDTH * 3;	// 16 RGB triplets

	for(int ctr=0;ctr<BITMAP_HEIGHT;ctr++)  {
		// Physical panel is only half the width of the virtual panel
		inptr += windowwidth;  			// Skip the first half of panel 1
		memcpy(outptr,inptr,windowwidth);	// Write the second half of panel 1
		inptr += windowwidth;			// Jockey past the block we've just written
		outptr += windowwidth;			// Jockey past the block we've just written

		// memcpy(outptr,inptr,windowwidth);	// Write the first half of panel 2
		inptr += windowwidth;			// Jockey past the block we've just written
		//outptr += windowwidth;			// Jockey past the block we've just written

		inptr += windowwidth; 			// Skip the second half
	}
	wiringPiSPIDataRW(0,spioutputbuf,769);
}
#endif
//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is mirrored on panel 2.
//
void Unicorn::update(PanelBitmap imgbitmap, uint32_t colour) {
	unsigned char *buffer = &framebuffer[0];
	unsigned char *ptr, pixels;
	unsigned char r=colour&0xff;
	unsigned char g=(colour>>8)&0xff;
	unsigned char b=(colour>>16)&0xff;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)


	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? b : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? r : 0;
				pixels <<= 1;
			}
		}

		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 1) ? b : 0;
				*buffer++ = (pixels & 1) ? g : 0;
				*buffer++ = (pixels & 1) ? r : 0;
				pixels >>= 1;
			}
		}
	}
}

//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is sent unaltered to both panels.
//
void Unicorn::update_nomirror(PanelBitmap imgbitmap, uint32_t colour) {
	unsigned char *buffer = &framebuffer[0];
	unsigned char *ptr, pixels;
	unsigned char r=colour&0xff;
	unsigned char g=(colour>>8)&0xff;
	unsigned char b=(colour>>16)&0xff;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)

	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? b : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? r : 0;
				pixels <<= 1;
			}
		}

		// Second panel (without mirroring)
		ptr = imgbitmap[h];
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? b : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? r : 0;
				pixels <<= 1;
			}
		}
	}
}

//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is mirrored on panel 2.
//
void Unicorn::overdub(PanelBitmap imgbitmap, uint32_t colour) {
	unsigned char *buffer = &framebuffer[0];
	unsigned char *ptr, pixels;
	unsigned char r=colour&0xff;
	unsigned char g=(colour>>8)&0xff;
	unsigned char b=(colour>>16)&0xff;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)

	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				if(pixels & 128) {
					buffer[0] = b;
					buffer[1] = g;
					buffer[2] = r;
				}
				buffer+=3;
				pixels <<= 1;
			}
		}

		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				if(pixels & 1) {
					buffer[0] = b;
					buffer[1] = g;
					buffer[2] = r;
				}
				buffer+=3;
				pixels >>= 1;
			}
		}
	}
}
