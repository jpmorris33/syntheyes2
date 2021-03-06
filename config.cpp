//
//  Configuration reader
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "inttype.h"

#include "syntheyes.hpp"

#include "eyeconfig.h"


void readConfig(FILE *fp);
void parse(const char *line);
uint32_t parseColour(const char *hex);

static char *nextWord(char *input);
static char *nextWordEnd(char *input);

STATES *getState(int id);
EXPRESSIONS *getExpression(const char *expression);
char serialPort[128];
bool rotated180 = false;

extern struct EXPRESSIONS expressionnames[];
extern struct STATES states[];

extern int LED_PIN;
extern int ACK_COUNT;
extern int COOLOFF_COUNT;
extern uint32_t eyeColour;
extern uint32_t cheekColour;
extern int drawmode;
extern int drawpattern;
extern bool cycle;
extern bool forcetransmitter;
extern int rainbowspeed;

extern int mapPin(int pin);
extern bool patch_raptorsden();
extern bool patch_raptorsden2();


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
char buf2[1024];
char *cmd;
char *param;
int temp;
STATES *stateptr;
EXPRESSIONS *expression;

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
if(!strcasecmp(cmd,"eyecolor:")) {
	eyeColour = parseColour(param);
	printf("Set eye color to 0x%06x\n",eyeColour);
}
if(!strcasecmp(cmd,"blushcolour:")) {
	cheekColour = parseColour(param);
	printf("Set cheek colour to 0x%06x\n",cheekColour);
}
if(!strcasecmp(cmd,"blushcolor:")) {
	cheekColour = parseColour(param);
	printf("Set cheek color to 0x%06x\n",cheekColour);
}
if(!strcasecmp(cmd,"cheekcolour:")) {
	cheekColour = parseColour(param);
	printf("Set cheek colour to 0x%06x\n",cheekColour);
}
if(!strcasecmp(cmd,"cheekcolor:")) {
	cheekColour = parseColour(param);
	printf("Set cheek color to 0x%06x\n",cheekColour);
}
if(!strcasecmp(cmd,"ackcount:")) {
	temp = atoi(param);
	if(temp > 0 ) {
		ACK_COUNT = temp;
		printf("Set status LED delay to %d ticks\n",ACK_COUNT);
	}
}
if(!strcasecmp(cmd,"cooloff:")) {
	temp = atoi(param);
	if(temp > 0 ) {
		COOLOFF_COUNT = temp;
		printf("Set cooloff between same expression to %d ticks\n",COOLOFF_COUNT);
	}
}
if(!strcasecmp(cmd,"setpin:")) {
	expression = getExpression(param);
	if(!expression) {
		printf("Set Pin: Invalid expression name '%s'\n",param);
	} else {
		stateptr = getState(expression->id);
		if(stateptr) {
			strcpy(buf2, nextWordEnd(param));
			temp=atoi(buf2);
			if(temp >= 0) {
				temp = mapPin(temp);
				if(temp >= 0) {
					stateptr->pin = temp;
					printf("Set Pin: expression '%s' to pin %d\n",expression->name,temp);
				} else {
					stateptr->pin = 0;
					printf("Set Pin: expression '%s' disabled\n",expression->name);
				}
			} else {
				printf("Set Pin: Invalid pin number '%s'\n",buf2);
			}
			
		} else {
			strcpy(buf2,param);
			nextWord(buf2);
			printf("Set Pin: Could not find expression matching '%s'\n",buf2);
		}
	}
}
if(!strcasecmp(cmd,"sprites:")) {
	strcpy(buf2,param);
	nextWord(buf2);

	if(!strcasecmp(buf2,"raptor")) {
		printf("Switching to the Raptor's Den sprite set\n");
		if(!patch_raptorsden()) {
			printf("Error patching sprites\n");
		}
	}

	if(!strcasecmp(buf2,"raptor2")) {
		printf("Switching to the Raptor's Den sprite set\n");
		if(!patch_raptorsden2()) {
			printf("Error patching sprites\n");
		}
	}

}
if(!strcasecmp(cmd,"serial:")) {
	strcpy(buf2,param);
	nextWord(buf2);
	strncpy(serialPort,buf2,128);
	serialPort[127]=0;
}
if(!strcasecmp(cmd,"transmitter:")) {
	strcpy(buf2,param);
	nextWord(buf2);

	if(!strcasecmp(buf2,"true")) {
		forcetransmitter=true;
	}
}
if(!strcasecmp(cmd,"rotate180:")) {
	strcpy(buf2,param);
	nextWord(buf2);

	if(!strcasecmp(buf2,"true")) {
		rotated180=true;
	}
}
if(!strcasecmp(cmd,"effect:")) {
	strcpy(buf2,param);
	nextWord(buf2);

	if(!strcasecmp(buf2,"cycle")) {
		printf("Eye will colour cycle\n");
		cycle=true;
	}

	if(!strcasecmp(buf2,"rainbow_v")) {
		printf("Eye will have a vertical rainbow effect\n");
		drawmode = DRAWMODE_RAINBOW;
		drawpattern = PATTERN_V;
	}

	if(!strcasecmp(buf2,"rainbow_h")) {
		printf("Eye will have a horizontal rainbow effect\n");
		drawmode = DRAWMODE_RAINBOW;
		drawpattern = PATTERN_H;
	}

	if(!strcasecmp(buf2,"rainbow_o")) {
		printf("Eye will have a square rainbow effect\n");
		drawmode = DRAWMODE_RAINBOW;
		drawpattern = PATTERN_O;
	}
}
if(!strcasecmp(cmd,"rainbowspeed:")) {
	temp = atoi(param);
	if(temp > 0 ) {
		rainbowspeed = temp;
		printf("Set rainbow delay to %d ms\n",rainbowspeed);
	}
}


}


uint32_t parseColour(const char *hex) {
uint32_t col = 0;

if(hex[0] == '#') hex++;
if(hex[0] == '0' && hex[1] == 'x') hex+=2;

sscanf(hex,"%x",&col);
return col;
}

EXPRESSIONS *getExpression(const char *expression) {
int ctr;
char buf[1024];
char *param;
strcpy(buf,expression);
param = nextWord(buf);

for(ctr=0;expressionnames[ctr].name;ctr++) {
	if(!strcasecmp(expressionnames[ctr].name,param)) {
		return &expressionnames[ctr];
	}
}
return NULL;
}

STATES *getState(int id) {
int ctr;
for(ctr=0;states[ctr].anim;ctr++) {
	if(states[ctr].id == id) {
		return &states[ctr];
	}
}
return NULL;
}


//
// String utils
//


char *nextWord(char *input) {
char *start=input,*end=NULL;
while(isspace(*start)) start++;
end=nextWordEnd(start);
*end=0;

return start;
}

char *nextWordEnd(char *input) {
char *start=input,*end=NULL;
while(isspace(*start)) start++;
end=start;
while(*end && !isspace(*end)) end++;
return end;
}
