//
//  Linux test harness
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../inttype.h"

#include "../syntheyes.hpp"
#include "../config.hpp"
#include "../NeopixelDriver.hpp"
#include "Unicorn.hpp"
#include "../ColourWheel.hpp"
#include "../MicDriver.hpp"
#include "../PosixTiming.hpp"

#include "eyeconfig.h"


extern PanelDriver *panel;
extern NeopixelDriver *statuslights;
extern MicDriver *adc;
extern ColourWheel colourwheel;
extern Timing *timing;

void initSynthEyes();
void loopSynthEyes();
void initPin(int pin);

extern PanelBitmap vfb;
extern PanelBitmap blushfb;
extern EyeBitmap framebuffer;
extern EyeBitmap blushbuffer;
extern bool updateL;
extern bool updateR;
bool transmitter = false;
bool forcetransmitter = false;
static EXPRESSIONS *forceExpression = NULL;

int LED_PIN = 0;
int ACK_COUNT = ACK_COUNT_DEFAULT;
int COOLOFF_COUNT = COOLOFF_DEFAULT;

uint32_t eyeColour = (EYECOLOUR_BLUE<<16)|(EYECOLOUR_GREEN<<8)|EYECOLOUR_RED;
uint32_t cheekColour = (BLUSHCOLOUR_BLUE<<16)|(BLUSHCOLOUR_GREEN<<8)|BLUSHCOLOUR_RED;

extern void errormsg(const char *msg);
extern void initFont();

int main(int argc, char *argv[]) {
	FILE *fp;

	panel = new Unicorn();
	panel->init();

	statuslights = new NeopixelDriver();
	statuslights->init(STATUS_PIN, STATUSPIXELS);

	timing = new PosixTiming();

	adc = NULL;
#ifdef VOICE_DETECTOR
	adc = new MicDriver();
	adc->init(VOICE_PIN);
#endif

	fp=fopen("./eyeconfig.txt","r");
	if(fp) {
		readConfig(fp);
		fclose(fp);
	}

	initSynthEyes();

	if(argc > 1) {
		if(!strcmp(argv[1],"receiver")) {
			transmitter=false;
		}
		if(!strcmp(argv[1],"transmitter")) {
			transmitter=true;
		}
		// Switch off the display (for testing)
		if(!strcmp(argv[1],"off")) {
			PanelBitmap blank;
			memset(blank,0,sizeof(blank));
			panel->update_nomirror(blank, 0);
			timing->wait_microseconds(100000);
			panel->draw();
			timing->wait_microseconds(100000);
			exit(0);
		}
		if(!strcmp(argv[1],"eyecolour") && argc > 2) {
			eyeColour = parseColour(argv[2]);

		}
		if(!strcmp(argv[1],"test") && argc > 2) {
			forceExpression = getExpression(argv[2]);

		}
	}

	if(forcetransmitter) {
		printf("Forcing into transmitter mode by config file\n");
		transmitter=true;
	}

	printVersion(SYNTHOS_VERSION, 5000);

	for(int ctr=0;ctr<2000;ctr++) {
		panel->draw();
		timing->wait_microseconds(1000);
	}

	while(1) {
		loopSynthEyes();
	}


}


void debug() {
 /*
        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);
*/
}

//
//  Draw the sprite on the 'right' display matrix
//

void drawEyeR() {
  unsigned char *ptr = &framebuffer[0][0];

  if(!updateR) {
    return;
  }

  memset(vfb,0,64);

  for(int row=0;row<16;row++) {
	vfb[row][2]=*ptr++;
	vfb[row][3]=*ptr++;
	}
  panel->update(vfb, eyeColour);

  memset(blushfb,0,64);

  ptr = &blushbuffer[0][0];
  for(int row=0;row<16;row++) {
	blushfb[row][2]=*ptr++;
	blushfb[row][3]=*ptr++;
	}
  panel->update(vfb, eyeColour);
  panel->overdub(blushfb, cheekColour);

  panel->draw();

  // Don't send again until the frame changes
  updateR=false;
  
}

//
//  Draw the mirrored sprite on the 'left' matrix (note the L&R panels are swapped)
//

void drawEyeL() {

//  unsigned char *ptr = &framebuffer[0][0];

  if(!updateL) {
    return;
  }

	// Do Nothing  

  // Don't send again until the frame changes
  updateL=false;
  
}

// Signal state to the other device
void transmit(char code) {
}

//
//  Default handler for Arduino, can be replaced for other platfirms
//

bool checkExpression(STATES *state) {

    if(forceExpression && forceExpression->id == state->id) {
        return true;
    }

    if(!state->pin) {
        return false;
    }

	SDL_PumpEvents();
	const unsigned char *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A + (state->code-'A')]) {
		return true;
	}
	if(keys[SDL_SCANCODE_ESCAPE]) {
		exit(1);
	}

    return false;
//    return !digitalRead(pin);
}

//
//  Callback for when we have an expression, this can be customised
//

void gotExpression(STATES *state) {
}

// Arduino compatibility wrapper

void randomSeed(unsigned int r) {
	srand(r);
}

int random(int lowest, int highest) {
highest-=lowest;
if(highest < 1) {
	highest=1;
}
return (random() % highest) + lowest;
}

void initPin(int pin) {
//	pullUpDnControl(pin,PUD_UP);
}

void setPin(int pin, bool state) {
//	if(pin >= 0) {
//		digitalWrite(pin,state);
//	}
}

int mapPin(int pin) {
return pin;
}
