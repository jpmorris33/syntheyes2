#include "ColourWheel.hpp"

ColourWheel::ColourWheel() {
	wheelR=60;
	wheelG=30;
	wheelB=10;
	wheelDirR=2;
	wheelDirG=-1;
	wheelDirB=1;
}

ColourWheel::ColourWheel(unsigned char r, unsigned char g, unsigned char b) {
	wheelR=r;
	wheelG=g;
	wheelB=b;
	wheelDirR=2;
	wheelDirG=-1;
	wheelDirB=1;
}

ColourWheel::ColourWheel(unsigned char r, unsigned char g, unsigned char b, char rdir, char gdir, char bdir) {
	wheelR=r;
	wheelG=g;
	wheelB=b;
	wheelDirR=rdir;
	wheelDirG=gdir;
	wheelDirB=bdir;
}


uint32_t ColourWheel::update() {

uint32_t colour = 0;
  
	colour |= (wheelR<<16);
	colour |= (wheelG<<8);
	colour |= wheelB;

	wheelR += wheelDirR;
	if(wheelR >= 254 || wheelR < 2) {
		wheelDirR=-wheelDirR;
	}
	wheelG += wheelDirG;
	if(wheelG >= 254 || wheelG < 2) {
		wheelDirG=-wheelDirG;
	}
	if(wheelB >= 254 || wheelB < 2) {
		wheelDirB=-wheelDirB;
	}
 
return colour;    
}
