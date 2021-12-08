/**
 * Render to a HUB75 panel using the Pico's programmable IO state machines.
 * Portions based on the Pico HUB75 example, Copyright (c) 2020 Raspberry Pi (Trading) Ltd under the BSD-3-Clause license
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Hub08.hpp"

#include <stdio.h>
#include <string.h>

#include "hardware/gpio.h"
#include "hub75.pio.h"

extern uint32_t rainbow[16]; // Colour table
static unsigned char rainbowpattern[16][16];

//
//	Init the PIO programs.
//	This is based on code from the Pico HUB75 example.
//
void Hub08::init() {
	pio = pio0;
	sm_data = 0;
	sm_row = 1;
	data_prog_offs = pio_add_program(pio, &hub75_data_rgb888_program);
	row_prog_offs = pio_add_program(pio, &hub75_row_program);
	hub75_data_rgb888_program_init(pio, sm_data, data_prog_offs, DATA_BASE_PIN, CLK_PIN);
	hub75_row_program_init(pio, sm_row, row_prog_offs, ROWSEL_BASE_PIN, ROWSEL_N_PINS, STROBE_PIN);
}

//
//	Put the framebuffer onto the HUB75 panel using the PIO state machines.
//	This is based on code from the Pico HUB75 example.
//
void Hub08::draw() {
	uint32_t *row,*rowptr;
	for (int rowsel = 0; rowsel < (1 << ROWSEL_N_PINS); ++rowsel) {
		row = &framebuffer[rowsel][0];
		for (int bit = 0; bit < 8; ++bit) {
			rowptr=row;
			hub75_data_rgb888_set_shift(pio, sm_data, data_prog_offs, bit);
			for (int x = 0; x < BITMAP_WIDTH; ++x) {
				pio_sm_put_blocking(pio, sm_data, *rowptr++);
				// FIXME: The Hub75 pio kernel is still trying to send a second row to R2,G2,B2
				pio_sm_put_blocking(pio, sm_data, 0);
			}
		        // Dummy pixel per lane
		        pio_sm_put_blocking(pio, sm_data, 0);
		        pio_sm_put_blocking(pio, sm_data, 0);
		        // SM is finished when it stalls on empty TX FIFO
		        hub75_wait_tx_stall(pio, sm_data);
		        // Also check that previous OEn pulse is finished, else things can get out of sequence
		        hub75_wait_tx_stall(pio, sm_row);

		        // Latch row data, pulse output enable for new row.
		        pio_sm_put_blocking(pio, sm_row, rowsel | (100u * (1u << bit) << 5));
		}
	}

}

//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is mirrored on panel 2.
//
void Hub08::update(PanelBitmap imgbitmap, uint32_t colour) {
	uint32_t *buffer = framebuffer[0];
	unsigned char *ptr, pixels;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)


	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? colour : 0;
				pixels <<= 1;
			}
		}

		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 1) ? colour : 0;
				pixels >>= 1;
			}
		}
	}
}

void Hub08::update_patterned(PanelBitmap imgbitmap, int offset) {
	uint32_t *buffer = framebuffer[0];
	unsigned char *ptr, pixels;
	uint32_t colour,index,xpos,ypos;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)

	ypos=0;
	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		xpos=0;
		// First panel
		for(ctr=0;ctr<wd8;ctr++) {

			index = (offset + (rainbowpattern[ypos][xpos]&0x0f))&0x0f;
			colour=rainbow[index]&0xff;
			xpos++;
			xpos &= 0x0f; // Constrain to 16 pixels

			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? colour : 0;
				pixels <<= 1;
			}
		}

		xpos=0;
		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {

			index = (offset + (rainbowpattern[ypos][xpos]&0x0f))&0x0f;
			colour=rainbow[index]&0xff;
			xpos++;
			xpos &= 0x0f; // Constrain to 16 pixels

			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 1) ? colour : 0;
				pixels >>= 1;
			}
		}

		ypos++;
		ypos &= 0x0f; // Constrain to 16 pixels
	}
}

//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is sent unaltered to both panels.
//
void Hub08::update_nomirror(PanelBitmap imgbitmap, uint32_t colour) {
	uint32_t *buffer = framebuffer[0];
	unsigned char *ptr, pixels;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)

	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? colour : 0;
				pixels <<= 1;
			}
		}

		// Second panel (without mirroring)
		ptr = imgbitmap[h];
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				*buffer++ = (pixels & 128) ? colour : 0;
				pixels <<= 1;
			}
		}
	}
}

//
//	Convert a 32x16 monochrome bitmap into a format the panel understands.  This image is mirrored on panel 2.
//
void Hub08::overdub(PanelBitmap imgbitmap, uint32_t colour) {
	uint32_t *buffer = framebuffer[0];
	unsigned char *ptr, pixels;
	int ctr,ctr2,wd8=BITMAP_WIDTH/16; // width divided by 8, but divided by 2 for 2 panels (e.g. divided by 16)

	for(int h=0;h<BITMAP_HEIGHT;h++)	{
		ptr = imgbitmap[h];

		// First panel
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *ptr++;
			for(ctr2=0;ctr2<8;ctr2++) {
				if(pixels & 128) {
					*buffer = colour;
				}
				buffer++;
				pixels <<= 1;
			}
		}

		// Second panel (mirrored)
		for(ctr=0;ctr<wd8;ctr++) {
			pixels = *(--ptr);
			for(ctr2=0;ctr2<8;ctr2++) {
				if(pixels & 1) {
					*buffer = colour;
				}
				buffer++;
				pixels >>= 1;
			}
		}
	}
}

//
//	Set the special effect pattern
//
void Hub08::set_pattern(unsigned char pattern[16][16]) {
	memcpy(rainbowpattern, pattern, 16*16);
}
