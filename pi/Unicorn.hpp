
#include "../PanelDriver.hpp"

class Unicorn : public PanelDriver {
	public:
		void init();
		void draw();
		void update(PanelBitmap bitmap, uint32_t colour);
		void update_patterned(PanelBitmap bitmap, int offset);
		void update_nomirror(PanelBitmap bitmap, uint32_t colour);
		void overdub(PanelBitmap bitmap, uint32_t colour);
		void set_pattern(unsigned char pattern[16][16]);
	private:
		unsigned char framebuffer[BITMAP_WIDTH*BITMAP_HEIGHT*3];
};
