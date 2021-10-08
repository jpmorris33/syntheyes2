#include "inttype.h"

#define BITMAP_WIDTH 64
#define BITMAP_HEIGHT 16

typedef unsigned char PanelBitmap[BITMAP_HEIGHT][BITMAP_WIDTH/16];

#define EYE_WIDTH 16
#define EYE_HEIGHT 16

typedef unsigned char EyeBitmap[EYE_HEIGHT][EYE_WIDTH/8];
