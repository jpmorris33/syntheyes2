#include "inttype.h"

class ColourWheel {

public:
	ColourWheel();
	ColourWheel(unsigned char r, unsigned char g, unsigned char b);
	ColourWheel(unsigned char r, unsigned char g, unsigned char b, char rdir, char gdir, char bdir);
	uint32_t update();
private:
  unsigned char wheelR;
  unsigned char wheelG;
  unsigned char wheelB;
  char wheelDirR;
  char wheelDirG;
  char wheelDirB;

	
};
