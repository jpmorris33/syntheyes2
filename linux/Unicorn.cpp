/**
 * Dummy unicorn panel driver with SDL output for testing
 */

#include <SDL2/SDL.h>

#include "Unicorn.hpp"

#include <stdio.h>
#include <string.h>

static SDL_Window *win;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static unsigned char outbuf[768];

//
//	Init the Unicorn HD driver
//
void Unicorn::init() {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);

	win = SDL_CreateWindow("",0,0,160,160,SDL_WINDOW_RESIZABLE);

	if (!win){
		printf("Failed to open window: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(win, -1, 0);
	if (!renderer){
		fprintf(stderr, "Count not open renderer, aborting\n");
		exit(1);
	}

	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_BGR24,SDL_TEXTUREACCESS_STREAMING,16,16);
	if (!texture){
		fprintf(stderr, "Failed to create texture\n");
		exit(1);
	}

	SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
	SDL_RenderClear( renderer );
}

//
//	Put the framebuffer onto the Unicorn HD panel
//
void Unicorn::draw() {

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	unsigned char *inptr = &framebuffer[0];
	unsigned char *outptr = &outbuf[0];

	int windowwidth = 16 * 3;	// 16 RGB triplets

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

    SDL_UpdateTexture(texture, NULL, &outbuf[0], windowwidth);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

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
