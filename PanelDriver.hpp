#include "BitmapType.hpp"

class PanelDriver {
	public:
		virtual void init();
		virtual void draw();
		virtual void update(PanelBitmap bitmap, uint32_t colour);
		virtual void update_nomirror(PanelBitmap bitmap, uint32_t colour);
		virtual void overdub(PanelBitmap bitmap, uint32_t colour);
};
