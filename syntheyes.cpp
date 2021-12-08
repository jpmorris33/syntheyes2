#include "inttype.h"

#include <string.h>
#include <stdlib.h>

#include "syntheyes.hpp"
#include "config.hpp"
#include "PanelDriver.hpp"
#include "NeopixelDriver.hpp"
#include "ColourWheel.hpp"
#include "MicDriver.hpp"
#include "Timing.hpp"

#include "eyeconfig.h"

#define CYCLE_EYES


// SynthOS 1.05
extern PanelBitmap initimg;
extern int LED_PIN;
extern int ACK_COUNT;
extern int COOLOFF_COUNT;
PanelDriver *panel;
NeopixelDriver *statuslights;
MicDriver *adc;
ColourWheel wheel;
Timing *timing;

void initSynthEyes();
void loopSynthEyes();
void set_pattern(int pattern);
extern void initPin(int pin);
extern void setPin(int pin, bool state);
extern uint32_t eyeColour;
extern void wait_microseconds(int us);
extern void transmit(char code);
extern bool transmitter;

#define STATUSBRIGHT 100
#define VOICEBRIGHT 255
#define FRAME_IN_MS 20  // Delay per animation frame in milliseconds (20 default)
#define WAIT_IN_MS  60  // Delay per tick in milliseconds when waiting to blink again (60 default)
#define MIN_DELAY    5   // Minimum delay between blinks
#define MAX_DELAY    250 // Maximum delay between blinks
#define STATUS_DIVIDER 16  // This controls the speed of the status light chaser, bigger is slower
#define CYCLE_DIVIDER 32  // This controls the speed of the eye colour shift, bigger is slower.  16 or less will glitch the display

#define STEPS (STATUSPIXELS*2)

// Functions

extern void drawEyeL();
extern void drawEyeR();
void getSprite(unsigned char *ptr, int blinkpos);
void wait(int ms, bool interruptable);
void getNextAnim();
extern bool checkExpression(STATES *state);
extern void gotExpression(STATES *state);
void statusCycle(unsigned char r, unsigned char g, unsigned char b);
void eyeCycle();
void cycle_null();
void cycle_colour();
void cycle_rainbow();

void randomSeed(unsigned int r);
int random(int lowest, int highest);

int eyeptr=0;
int frameidx=0;
int overlayidx=0;
signed char *eyeanim;
signed char *overlayanim;
int eyemax = 0;
int waittick=0;
int state=0;
int blinkidx=0;
int blinkdir=0;
int nextstate=0;  // For queueing user-triggered states
int laststate=-1;
int cooloff = 0;
int ack=0;
int drawmode=DRAWMODE_NORMAL;
int drawpattern=PATTERN_V;
bool cycle = false;
int rainbow_offset=0;
bool updateL=true;
bool updateR=true;
unsigned char ramp[STEPS];
void (*update_cycle)(void) = cycle_null;

PanelBitmap vfb;
PanelBitmap blushfb;

// Frame buffer for procedural blink
EyeBitmap framebuffer;
EyeBitmap blushbuffer;

// Rainbow effect
uint32_t rainbow[16] = {0xff1700,0xff7200,0xffce00,0xe8ff00,0x79ff00,0x1fff00,0x00ff3d,0x00ff98,0x00fff4,0x00afff,0x0054ff,0x0800ff,0x6300ff,0xbe00ff,0xff00e4,0xff0089};
int rainbowspeed = 10;

#include "patterns.h"

//
//  Animation data
//

#include "images-xerian.h"
#include "images-arthi.h"


// Add any new animation triggers here

struct STATES states[] = {
{BLINK,     closeeye,    sizeof(closeeye), NULL,		0, 		'B'},
{WINK,      closeeye,    sizeof(closeeye), NULL,		0,	 	'W'},
{ROLLEYE,   rolleye,     sizeof(rolleye),  NULL,		EYEROLL_PIN, 	'R'},
{STARTLED,  startled,    sizeof(startled), NULL,		STARTLED_PIN, 	'S'},
{ANNOYED,   annoyed,     sizeof(annoyed),  NULL,		ANNOYED_PIN, 	'A'},
{BLUSHING,  blushing,    sizeof(blushing), ovl_blushing,	BLUSHING_PIN, 	'L'},
{OWO,       owo,       	 sizeof(owo),      NULL,		OWO_PIN,	'O'},
{FAULT,     fault,       sizeof(fault),    ovl_fault,		FAULT_PIN,	'F'},
{HAPPY,     happy,       sizeof(happy),    NULL,		HAPPY_PIN,	'H'},
// DO NOT REMOVE THIS LAST LINE!
{0,         NULL,        0,                NULL,		0,		0}  
};

// Name lookup for the config parser
struct EXPRESSIONS expressionnames[] = {
	{ "rolleye",	ROLLEYE },
	{ "eyeroll",	ROLLEYE },
	{ "startled",	STARTLED },
	{ "annoyed",	ANNOYED },
	{ "blushing",	BLUSHING },
	{ "blush",	BLUSHING },
	{ "owo",	OWO },
	{ "fault",	FAULT },
	{ "happy",	HAPPY },
	{ "blink",	BLINK },
	{ NULL,		0 }
};


void initSynthEyes() {
  eyeanim = &closeeye[0];
  overlayanim = NULL;
  eyemax = sizeof(closeeye);
  state = BLINK;

	// Init pins for states
	for(int ctr=0;states[ctr].anim;ctr++) {
		if(states[ctr].pin) {
			initPin(states[ctr].pin);
			}
	}

  // Build lookup table for pulsating status lights
  // I'm sure there's a smarter way to do this on the fly, but...
	int ramping=0;
	for(int ctr=0;ctr<STEPS;ctr++) {
		ramp[ctr]=ramping;
		if(ctr<STEPS/2) {
			ramping+=(256/(STEPS*2));
		} else {
			ramping-=(256/(STEPS*2));
		}
	}

  if(cycle) {
     update_cycle = cycle_colour;
  }
  if(drawmode == DRAWMODE_RAINBOW) {
     update_cycle = cycle_rainbow;
  }

  set_pattern(drawpattern);


  // Initial draw
  getSprite(&eye[frameidx][0], 0);
  drawEyeR();
  drawEyeL();

  statusCycle(STATUSCOLOUR_RED,STATUSCOLOUR_GREEN,STATUSCOLOUR_BLUE);

}

void loopSynthEyes() {
    // Draw the sprites
    getSprite(&eye[frameidx][0], blinkidx);
    drawEyeR();
    getSprite(&eye[frameidx][0], state == WINK ? 0 : blinkidx);
    drawEyeL();
    setPin(LED_PIN, ack > 0);
    if(ack > 0) ack--;
    if(cooloff > 0) cooloff--;

  // If we're idling, count down
  if(waittick > 0) {
    if(state == BLINK || state == WINK) {
      wait(FRAME_IN_MS/2,false); // Can't interrupt a blink and it runs twice as fast as usual animations
    } else {

      // If we're waiting for the other unit to signal, reset the wait period
      if(state == WAITING && !transmitter) {
        waittick = 16000; // Wait forever since this will be reset
      }
      wait(WAIT_IN_MS,true); // Can interrupt
    }
    waittick--;
  } else {
    if(state == WAITING) {
      getNextAnim();
    }
    
    // Otherwise, update the animation
    updateL=updateR=true;
    wait(FRAME_IN_MS,false);
    
    eyeptr++;
    if(eyeptr >= eyemax || nextstate) {
      // If we've hit the end, go back to the start and wait
      eyeptr=0;
      // Wait between 5-250 cycles before blinking again
      waittick = random(MIN_DELAY,MAX_DELAY);
      laststate = state;
      cooloff = COOLOFF_COUNT;
      state = WAITING;
    }

    // Negative is pause in cycles
    if(eyeanim[eyeptr] < 0) {
      waittick = -eyeanim[eyeptr];
      return;
    }

    frameidx=eyeanim[eyeptr];
    if(overlayanim) {
        overlayidx = overlayanim[eyeptr];
    }
  }
  
  // Handle blinking
  if(state == BLINK || state == WINK) {
    blinkidx += blinkdir;
    if(blinkidx > 20) {
      blinkdir = -1;
    }
    if(blinkidx < 1) {
      blinkidx=0;
      blinkdir=0;
      // Wait between 5-250 cycles before blinking again
      waittick = random(MIN_DELAY,MAX_DELAY);
      state = WAITING;
    }
    
    updateL=updateR=true;
  }
  
  
}

//
//  Pick the animation to roll, which may have been cued up in response to a GPIO pin
//

void getNextAnim() {
  int ctr;

  eyeptr=0;
  state = nextstate;

  for(ctr=0;states[ctr].anim;ctr++) {
    if(states[ctr].id == nextstate) {
      eyeanim = states[ctr].anim;
      overlayanim = states[ctr].overlay;
      eyemax = states[ctr].animlen;
      if(state != BLINK) {
          transmit(states[ctr].code);
      }
      break;
    }
  }
  nextstate = BLINK;

  if(state == BLINK) {
    // 1 in 10 chance of him winking
    ctr = random(1,10);
    if(ctr == 1) {
      state = WINK;
      transmit(states[1].code); // Wink
    } else {
      transmit(states[0].code); // Blink
    }
    blinkdir=1;
  }

}

//
//  Get the requested animation frame and apply a blink effect if necessary
//

void getSprite(unsigned char *ptr, int blinkpos) {
  memcpy(framebuffer,ptr,32);
  if(blinkpos > 0) {
    if(blinkpos > 16) {
      blinkpos = 16;
    }
    memset(framebuffer,0,blinkpos * 2);
  }

  if(overlayanim) {
	memcpy(blushbuffer,overlay[overlayidx],32);
  } else {
	memset(blushbuffer,0,32);
  }

}

//
//  Wait, and cue up a reaction if we detect one via GPIO
//

void wait(int ms, bool interruptable) {
	timing->set(ms);
	while(!timing->elapsed()) {
		panel->draw();
		timing->wait_microseconds(50);

		statusCycle(STATUSCOLOUR_RED,STATUSCOLOUR_GREEN,STATUSCOLOUR_BLUE);
		if(state == WAITING) {
			for(int ctr2=0;states[ctr2].anim;ctr2++) {
				if(checkExpression(&states[ctr2])) {
					// Did we just do it?
					if(states[ctr2].id == laststate && cooloff > 0) {
						continue;
					}
					// Flash the status LED, but only if we're not doing a blink
					if(states[ctr2].id != BLINK && states[ctr2].id != WINK) {
						ack=ACK_COUNT;
						gotExpression(&states[ctr2]);
					}
					nextstate = states[ctr2].id;
						if(interruptable) {
						waittick=0;
					return;
					}
				}
			}
		}
	}
}

void statusCycle(unsigned char r, unsigned char g, unsigned char b) {
  static uint16_t pos=0;
  static int divider=0;
  int maxdiv=STATUS_DIVIDER;

#ifdef STATUS_LIGHTS
  bool bright = false;

  if(adc) {
    bright = !(adc->read() > ADC_THRESHOLD);
  }
#endif

  divider++;
  if(divider > maxdiv) {
    pos++;
    divider=0;
  }
  if(pos > 255) {
    pos=0;
  }

#ifdef STATUS_LIGHTS
  for(int ctr=0; ctr< STATUSPIXELS; ctr++) {
    if(bright) {
      statuslights->setpixel(ctr, STATUSFLASH_RED, STATUSFLASH_GREEN, STATUSFLASH_BLUE, 255); // Flash at full brightness
    } else {
      statuslights->setpixel(ctr, r, g, b, ramp[(ctr+pos)%STEPS]);
    }
  }
	statuslights->draw();
#endif

eyeCycle();

}

void eyeCycle() {
  static int divider=0;
  int maxdiv=CYCLE_DIVIDER;

  divider++;
  if(divider > maxdiv) {
    divider=0;
   update_cycle();
  }
}

void cycle_null() {
}

void cycle_colour() {
	static int divider=0;
	divider++;
	if(divider < rainbowspeed) {
		return;
	}
	divider=0;

	eyeColour = wheel.update();
	updateR=true;
	drawEyeR();
}

void cycle_rainbow() {
	static int divider=0;
	divider++;
	if(divider < rainbowspeed) {
		return;
	}
	divider=0;

	rainbow_offset++;
	rainbow_offset &= 0x0f;
	updateR=true;
	drawEyeR();
}


//
//	Patch the sprite set at runtime.
//	This is a bit hacky, but quicker and easier than redesigning everything.
//

void patch_state(int id, signed char *anim, int len, signed char *ovl) {
	STATES *s=getState(id);
	if(!s) {
		return;
	}
	s->anim = anim;
	s->animlen = len;
	s->overlay = ovl;
}

#define PATCH_RAPTORSDEN(a,b,c) patch_state(a,rd_##b,sizeof(rd_##b),c);

bool patch_raptorsden() {

	if(sizeof(rd_eye) > sizeof(eye)) {
		return false;
	}
	if(sizeof(rd_overlay) > sizeof(overlay)) {
		return false;
	}

	PATCH_RAPTORSDEN(BLINK,closeeye,NULL);
	PATCH_RAPTORSDEN(WINK,closeeye,NULL);
	PATCH_RAPTORSDEN(ROLLEYE,rolleye,NULL);
	PATCH_RAPTORSDEN(STARTLED,startled,NULL);
	PATCH_RAPTORSDEN(ANNOYED,annoyed,NULL);
	PATCH_RAPTORSDEN(BLUSHING,blushing,rd_ovl_blushing);
	PATCH_RAPTORSDEN(OWO,owo,NULL);
	PATCH_RAPTORSDEN(FAULT,fault,rd_ovl_fault);
	PATCH_RAPTORSDEN(HAPPY,happy,NULL);

	// Assume Arthi's sprite data is smaller than Xerian's
	memcpy(eye,rd_eye,sizeof(rd_eye));
	memcpy(overlay,rd_overlay,sizeof(rd_overlay));

	return true;
}


void set_pattern(int pattern) {
	switch(pattern) {
		case PATTERN_H:
			panel->set_pattern(pattern_h);
			break;
		case PATTERN_O:
			panel->set_pattern(pattern_o);
			break;
		default:
			panel->set_pattern(pattern_v);
			break;
	}
}
