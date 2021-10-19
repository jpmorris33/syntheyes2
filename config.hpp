//
//  Configuration reader - configuration defaults is in eyeconfig.h
//

#include <stdio.h>

extern void readConfig(FILE *fp);
extern void parse(const char *line);
extern uint32_t parseColour(const char *hex);
extern EXPRESSIONS *getExpression(const char *name);
extern STATES *getState(int id);
