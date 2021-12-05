#include "pico/stdlib.h"
#include <string.h>
#include <stdlib.h>

#include "../syntheyes.hpp"
#include "WS2812.hpp"
#include "Hub08.hpp"
#include "../ColourWheel.hpp"
#include "PicoMic.hpp"
#include "PicoTiming.hpp"

#include "eyeconfig.h"

// SynthOS 1.04
PanelBitmap initimg = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11101010, 0b11101011, 0b10101011, 0b10111000, 
    0b10001010, 0b10101001, 0b00101010, 0b10100000, 
    0b01001110, 0b10101001, 0b00111010, 0b10111000, 
    0b00100100, 0b10101001, 0b00101010, 0b10001000, 
    0b11100100, 0b10111001, 0b00101011, 0b10111000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00001000, 0b00000000, 0b00111100, 0b01111100, 
    0b00011000, 0b00000000, 0b01000010, 0b01000000, 
    0b00101000, 0b00000000, 0b01000010, 0b01000000, 
    0b00001000, 0b00000000, 0b01000010, 0b0111100, 
    0b00001000, 0b00000000, 0b01000010, 0b00000100, 
    0b00001000, 0b00110000, 0b01000010, 0b00000100, 
    0b00111100, 0b00110000, 0b00111100, 0b01111000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
};



int LED_PIN = PICO_DEFAULT_LED_PIN;
int ACK_COUNT = ACK_COUNT_DEFAULT;
int COOLOFF_COUNT = COOLOFF_DEFAULT;

extern PanelDriver *panel;
extern NeopixelDriver *statuslights;
extern MicDriver *adc;
extern ColourWheel colourwheel;
extern Timing *timing;

void initSynthEyes();
void loopSynthEyes();
void initPin(int pin);
extern bool patch_raptorsden();

extern PanelBitmap vfb;
extern PanelBitmap blushfb;
extern EyeBitmap framebuffer;
extern EyeBitmap blushbuffer;
extern bool updateL;
extern bool updateR;
bool transmitter = false;


uint32_t eyeColour = (EYECOLOUR_BLUE<<16)|(EYECOLOUR_GREEN<<8)|EYECOLOUR_RED;
uint32_t cheekColour = (BLUSHCOLOUR_BLUE<<16)|(BLUSHCOLOUR_GREEN<<8)|BLUSHCOLOUR_RED;

int main() {
	stdio_init_all();
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	timing = new PicoTiming();

	panel = new Hub08();
	panel->init();

	statuslights = new WS2812();
	statuslights->init(STATUS_PIN, STATUSPIXELS);

	adc = NULL;
#ifdef VOICE_DETECTOR
	adc = new PicoMic();
	adc->init(VOICE_PIN);
#endif

	#ifdef IMAGES_ARTHI
		patch_raptorsden();
	#endif

	initSynthEyes();

        gpio_put(LED_PIN, 1);

	panel->update_nomirror(initimg, 0x808080);
	for(int ctr=0;ctr<2000;ctr++) {
		panel->draw();
		sleep_ms(1);
	}

	while(1) {
		loopSynthEyes();
	}


}


void debug() {
 
        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);

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

  unsigned char *ptr = &framebuffer[0][0];

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
    if(!state->pin) {
        return false;
    }
    return !gpio_get(state->pin);
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
	gpio_set_dir(pin, GPIO_IN);
	gpio_pull_up(pin);
}

void setPin(int pin, bool state) {
	gpio_put(pin,state);
}

int mapPin(int pin) {
	return pin;
}
