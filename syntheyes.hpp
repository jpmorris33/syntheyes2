

#define SYNTHOS_VERSION "1.07"

//
//  State structure
//

struct STATES {
  char id;
  signed char *anim;
  unsigned char animlen;
  signed char *overlay;
  char pin;
  char code;
};

struct EXPRESSIONS {
  const char *name;
  int id;
};

extern void errormsg(const char *msg);
extern void initFont();
extern void printVersion(const char *version, int duration);

// System state variables

#define WAITING -1
#define BLINK 0
#define WINK 1
#define ROLLEYE 2
#define STARTLED 3
#define ANNOYED 4
#define BLUSHING 5
#define FAULT 6
#define OWO 7
#define HAPPY 8

// Draw modes

#define DRAWMODE_NORMAL 0
#define DRAWMODE_RAINBOW 1

// Rainbow patterns

#define PATTERN_V 0
#define PATTERN_H 1
#define PATTERN_O 2
