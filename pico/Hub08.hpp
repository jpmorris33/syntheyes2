
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "../PanelDriver.hpp"

#define DATA_BASE_PIN 0
#define DATA_N_PINS 6
#define ROWSEL_BASE_PIN 6
#define ROWSEL_N_PINS 4	// 32x16 = 3 rows, ??x32 = 4 etc
#define CLK_PIN 11
#define STROBE_PIN 12
#define OEN_PIN 13

class Hub08 : public PanelDriver {
	public:
		void init();
		void draw();
		void update(PanelBitmap bitmap, uint32_t colour);
		void update_nomirror(PanelBitmap bitmap, uint32_t colour);
		void overdub(PanelBitmap bitmap, uint32_t colour);
	private:
		PIO pio;
		uint sm_data;
		uint sm_row;
		uint data_prog_offs;
		uint row_prog_offs;

		uint32_t framebuffer[BITMAP_HEIGHT][BITMAP_WIDTH];
		uint32_t rowbuf[2][BITMAP_WIDTH];
};
