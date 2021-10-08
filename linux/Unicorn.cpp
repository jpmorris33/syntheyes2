/**
 * Dummy unicorn panel driver with no SPI transfer
 */

#include "Unicorn.hpp"

#include <stdio.h>
#include <string.h>

unsigned char spioutputbuf[769];

//
//	Init the Unicorn HD driver
//
void Unicorn::init() {
}

//
//	Put the framebuffer onto the Unicorn HD panel
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
}

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
				*buffer++ = (pixels & 128) ? r : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? b : 0;
				pixels <<= 1;
			}
		}

		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 1) ? r : 0;
				*buffer++ = (pixels & 1) ? g : 0;
				*buffer++ = (pixels & 1) ? b : 0;
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
				*buffer++ = (pixels & 128) ? r : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? b : 0;
				pixels <<= 1;
			}
		}

		// Second panel (without mirroring)
		ptr = imgbitmap[h];
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? r : 0;
				*buffer++ = (pixels & 128) ? g : 0;
				*buffer++ = (pixels & 128) ? b : 0;
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
					buffer[0] = r;
					buffer[1] = g;
					buffer[2] = b;
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
					buffer[0] = r;
					buffer[1] = g;
					buffer[2] = b;
				}
				buffer+=3;
				pixels >>= 1;
			}
		}
	}
}
