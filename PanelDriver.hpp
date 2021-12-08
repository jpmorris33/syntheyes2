#include "BitmapType.hpp"

class PanelDriver {
	public:
		virtual void init();
		virtual void draw();
		virtual void update(PanelBitmap bitmap, uint32_t colour);
		virtual void update_patterned(PanelBitmap bitmap, int offset);
		virtual void update_nomirror(PanelBitmap bitmap, uint32_t colour);
		virtual void overdub(PanelBitmap bitmap, uint32_t colour);
		virtual void set_pattern(unsigned char pattern[16][16]);
};
