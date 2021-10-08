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
#include "../NeopixelDriver.hpp"
#include "Unicorn.hpp"
#include "../ColourWheel.hpp"
#include "../MicDriver.hpp"
#include "../PosixTiming.hpp"

#include "eyeconfig.h"


// SynthOS 1.00
PanelBitmap initimg = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11110111, 0b11111101, 0b11110111, 0b11111101, 
    0b10010101, 0b01010101, 0b10010101, 0b01010101, 
    0b11010101, 0b01010111, 0b11010101, 0b01010111, 
    0b01001001, 0b01010101, 0b01001001, 0b01010101, 
    0b11001001, 0b01010101, 0b11001001, 0b01010101, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b11101101, 0b01110111, 0b11101101, 0b01110111, 
    0b10101001, 0b01010101, 0b10101001, 0b01010101, 
    0b10101101, 0b01010101, 0b10101101, 0b01010101, 
    0b10100101, 0b01010101, 0b10100101, 0b01010101, 
    0b11101101, 0b01110111, 0b11101101, 0b01110111, 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    0b00001101, 0b10101000, 0b01010111, 0b11010111, 
    0b00001001, 0b00101000, 0b00100101, 0b01010010, 
    0b00001001, 0b10010000, 0b01010100, 0b01010010, 
};

extern PanelDriver *panel;
extern NeopixelDriver *statuslights;
extern MicDriver *adc;
extern ColourWheel colourwheel;
extern Timing *timing;

void initSynthEyes();
void loopSynthEyes();
void initPin(int pin);
void readConfig(FILE *fp);
void parse(const char *line);
uint32_t parseColour(const char *hex);

extern PanelBitmap vfb;
extern PanelBitmap blushfb;
extern EyeBitmap framebuffer;
extern EyeBitmap blushbuffer;
extern bool updateL;
extern bool updateR;
extern STATES states[];

static char serialState=0;

uint LED_PIN = 0;

uint32_t eyeColour = (EYECOLOUR_RED<<16)|(EYECOLOUR_GREEN<<8)|EYECOLOUR_BLUE;
uint32_t cheekColour = (BLUSHCOLOUR_RED<<16)|(BLUSHCOLOUR_GREEN<<8)|BLUSHCOLOUR_BLUE;

int serialfd;
bool transmitter = true;

int main(int argc, char *argv[]) {
	FILE *fp;

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
	}

	serialfd=serialOpen("/dev/ttyAMA0", 19200);
	if(serialfd != -1) {
		if(transmitter) {
			printf("Opened serial for transmission\n");
		} else {
			printf("Opened serial for reception\n");
		}
	}

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
	if(transmitter && serialfd != -1) {
		serialPutchar(serialfd, code);
//		printf("Transmitted code '%d'\n",code);
	}
}


//
//  Default handler for Arduino, can be replaced for other platfirms
//

bool checkExpression(STATES *state) {
	char serialIn=0;

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
	pullUpDnControl(pin,PUD_UP);
}

void setPin(int pin, bool state) {
	digitalWrite(pin,state);
}

//
//  Config reader
//

void readConfig(FILE *fp) {
char buf[1024];

for(;;) {
	if(feof(fp)) {
		return;
	}
	buf[0]=0;
	fgets(buf,1024,fp);
	parse(buf);
}

}


void parse(const char *line) {
char buf[1024];
char *cmd;
char *param;

if(line[0] == '#') {
	return;
}

strcpy(buf,line);

cmd=buf;
param=strchr(buf,' ');
if(!param) {
	return;
	}
*param++=0;
while(*param == ' ') param++;

if(!strcasecmp(cmd,"eyecolour:")) {
	eyeColour = parseColour(param);
	printf("Set eye colour to 0x%06x\n",eyeColour);
}

if(!strcasecmp(cmd,"blushcolour:")) {
	cheekColour = parseColour(param);
	printf("Set cheek colour to 0x%06x\n",cheekColour);
}
if(!strcasecmp(cmd,"cheekcolour:")) {
	cheekColour = parseColour(param);
	printf("Set cheek colour to 0x%06x\n",cheekColour);
}


}


uint32_t parseColour(const char *hex) {
uint32_t col = 0;

if(hex[0] == '#') hex++;
if(hex[0] == '0' && hex[1] == 'x') hex+=2;

sscanf(hex,"%x",&col);
return col;
}
