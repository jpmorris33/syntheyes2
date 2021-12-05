//
//  Raspberry Pi
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "../inttype.h"

#include "../syntheyes.hpp"
#include "../config.hpp"
#include "../NeopixelDriver.hpp"
#include "Unicorn.hpp"
#include "../ColourWheel.hpp"
#include "../MicDriver.hpp"
#include "../PosixTiming.hpp"

#include "eyeconfig.h"


// SynthOS 1.04
PanelBitmap initimg = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11010101, 0b10111101, 0b11010101, 0b10111101, 
    0b10010101, 0b01010101, 0b10010101, 0b01010101, 
    0b11010101, 0b01010111, 0b11010101, 0b01010111, 
    0b01001001, 0b01010101, 0b01001001, 0b01010101, 
    0b11001001, 0b01010101, 0b11001001, 0b01010101, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b10100100, 0b00100111, 0b10100100, 0b00100111, 
    0b10101100, 0b01010100, 0b10101100, 0b01010100, 
    0b10100100, 0b01010110, 0b10100100, 0b01010110, 
    0b10100100, 0b01010001, 0b10100100, 0b01010001, 
    0b01000101, 0b00100110, 0b01000101, 0b00100110, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00001101, 0b10101000, 0b01010111, 0b11010111, 
    0b00001001, 0b00101000, 0b00100101, 0b01010010, 
    0b00001001, 0b10010000, 0b01010100, 0b01010010, 
};

// Comms error
PanelBitmap serialerr = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11101110, 0b11111000, 0b11101110, 0b11111000,
    0b10001010, 0b10101000, 0b10001010, 0b10101000, 
    0b10001010, 0b10101000, 0b10001010, 0b10101000, 
    0b10001010, 0b10101000, 0b10001010, 0b10101000, 
    0b11101110, 0b10101000, 0b11101110, 0b10101000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11101110, 0b11100110, 0b11101110, 0b11100110, 
    0b10001010, 0b10100110, 0b10001010, 0b10100110, 
    0b11101100, 0b11000110, 0b11101100, 0b11000110, 
    0b10001010, 0b10100000, 0b10001010, 0b10100000, 
    0b11101010, 0b10100110, 0b11101010, 0b10100110, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
};

extern PanelDriver *panel;
extern NeopixelDriver *statuslights;
extern MicDriver *adc;
extern ColourWheel colourwheel;
extern Timing *timing;

void initSynthEyes();
void loopSynthEyes();
void initPin(int pin);
extern bool patch_arthi();

extern PanelBitmap vfb;
extern PanelBitmap blushfb;
extern EyeBitmap framebuffer;
extern EyeBitmap blushbuffer;
extern bool updateL;
extern bool updateR;
extern STATES states[];
extern char serialPort[128];

static char serialState=0;
static EXPRESSIONS *forceExpression = NULL;

int LED_PIN = 0;
int ACK_COUNT = ACK_COUNT_DEFAULT;
int COOLOFF_COUNT = COOLOFF_DEFAULT;
extern int drawmode;
extern int rainbow_offset;
uint32_t eyeColour = (EYECOLOUR_RED<<16)|(EYECOLOUR_GREEN<<8)|EYECOLOUR_BLUE;
uint32_t cheekColour = (BLUSHCOLOUR_RED<<16)|(BLUSHCOLOUR_GREEN<<8)|BLUSHCOLOUR_BLUE;

int serialfd;
bool transmitter = true;

int main(int argc, char *argv[]) {
	FILE *fp;

	strcpy(serialPort,"/dev/ttyAMA0");

	unsigned long colour=0x404040;
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

	fp=fopen("/boot/eyeconfig.txt","r");
	if(fp) {
		readConfig(fp);
		fclose(fp);
	}

	initSynthEyes();

	// if pin 29 (BCM5 - 21 in WiringPi) is grounded, we're the transmitter
	initPin(21);
	timing->wait_microseconds(250);
	if(digitalRead(21)) {
		transmitter=false;
	}

	if(argc > 1) {
		if(!strcmp(argv[1],"receiver")) {
			transmitter=false;
		}
		if(!strcmp(argv[1],"transmitter")) {
			transmitter=true;
		}
		// Switch off the display (for testing)
		if(!strcmp(argv[1],"off")) {
			panel->update_nomirror(initimg, 0);
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

	printf("Opening serial port '%s'\n", serialPort);
	serialfd=serialOpen(serialPort, 19200);
	if(serialfd != -1) {
		if(transmitter) {
			printf("Opened serial for transmission\n");
		} else {
			printf("Opened serial for reception\n");
		}
	} else {
		printf("Error opening serial port '%s'\n", serialPort);
		panel->update_nomirror(serialerr, 0x00ff0000);
		while(1) {
			panel->draw();
			timing->wait_microseconds(1000);
		}
	}

	// If we're the receiver, set up pin 40 as the ACK LED, and pin 39 to echo FAULT to the status light processor
	if(!transmitter) {
		LED_PIN = 29; // GPIO21, physical pin 40
		pinMode(LED_PIN, OUTPUT);
	} else {
		pinMode(FAULT_ECHO_PIN, OUTPUT); // physical pin 35
		digitalWrite(FAULT_ECHO_PIN, 1);
	}

	#ifdef IMAGES_ARTHI
		patch_raptorsden();
	#endif

	panel->update_nomirror(initimg, colour);
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
  if(!drawmode) {
	  panel->update(vfb, eyeColour);
  } else {
	if(drawmode == DRAWMODE_RAINBOW_V) {
	  panel->update_rainbowV(vfb, rainbow_offset);
	} else {
	  panel->update_rainbowH(vfb, rainbow_offset);
	}
  }

  memset(blushfb,0,64);

  ptr = &blushbuffer[0][0];
  for(int row=0;row<16;row++) {
	blushfb[row][2]=*ptr++;
	blushfb[row][3]=*ptr++;
	}
//  panel->update(vfb, eyeColour);
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
	if(transmitter && serialfd != -1) {
		serialPutchar(serialfd, code);
//		printf("Transmitted code '%d'\n",code);
	}
}


//
//  Default handler for Arduino, can be replaced for other platforms
//  This also implements the Receiver functionality when using two Pis
//

bool checkExpression(STATES *state) {
	char serialIn=0;

    if(forceExpression && forceExpression->id == state->id) {
        return true;
    }

    if((!serialState) && (!transmitter) && serialDataAvail(serialfd) > 0) {
//	printf("reading serial\n");
	serialIn = serialGetchar(serialfd);
//	printf("got %d\n", serialIn);
	// Only use if it exists
	for(int ctr=0;states[ctr].anim;ctr++) {
		if(states[ctr].code == serialIn) {
			serialState = serialIn;
		}
	}
    }

    if(serialState) {
        if(serialState == state->code) {
		serialState = 0;
		return true;
	}
    }

    // Stop the receiver doing things on its own
    if(!transmitter) {
        return false;
    }

    if(!state->pin) {
        return false;
    }
    return !digitalRead(state->pin);
}



//
//  Callback for when we have an expression, this can be customised
//

void gotExpression(STATES *state) {

    // echo the fault command to the status light processor
    if(transmitter && state->id == FAULT) {
	digitalWrite(FAULT_ECHO_PIN, 0);
	timing->wait_microseconds(8000);  // Give it 8 milliseconds
	digitalWrite(FAULT_ECHO_PIN, 1);
        return;
    }

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
	pinMode(pin,INPUT);
	pullUpDnControl(pin,PUD_UP);
}

void setPin(int pin, bool state) {
	if(pin >= 0) {
		digitalWrite(pin,state);
	}
}

//
//  Translate physical pins to WiringPi pins
//

int mapPin(int pin) {

switch(pin)	{
	case 40:
		return 29;
	//   39 is ground
	case 38:
		return 28;
	case 37:
		return 25;
	case 36:
		return 27;
// 35 is reserved
//	case 35:
//		return 24;
	//   34 is ground
	case 33:
		return 23;
	case 32:
		return 26;
	case 31:
		return 22;
	//   30 is ground
// 29 is reserved too
//	case 29:
//		return 21;
	default:
		return -1;
};

}
