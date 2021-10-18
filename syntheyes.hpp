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
