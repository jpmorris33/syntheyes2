//
//  Configuration reader
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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

extern struct EXPRESSIONS expressionnames[];
extern struct STATES states[];

extern int LED_PIN;
extern int ACK_COUNT;
extern int COOLOFF_COUNT;
extern uint32_t eyeColour;
extern uint32_t cheekColour;

extern int mapPin(int pin);


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
while(*start == ' ') start++;
end=nextWordEnd(start);
*end=0;

return start;
}

char *nextWordEnd(char *input) {
char *start=input,*end=NULL;
while(*start == ' ') start++;
end=start;
while(*end != ' ' && *end) end++;
return end;
}
