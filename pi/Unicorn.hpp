
#include "../PanelDriver.hpp"

class Unicorn : public PanelDriver {
	public:
		void init();
		void draw();
		void update(PanelBitmap bitmap, uint32_t colour);
		void update_nomirror(PanelBitmap bitmap, uint32_t colour);
		void overdub(PanelBitmap bitmap, uint32_t colour);
	private:
		unsigned char framebuffer[BITMAP_WIDTH*BITMAP_HEIGHT*3];
};
