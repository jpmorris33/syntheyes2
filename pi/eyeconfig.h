//
//	Configuration - raspberry pi
//

#ifndef EYE_CONFIG

// Pick one
#define IMAGES_XERIAN
//#define IMAGES_ARTHI

// Duration of acknowledgement LED
#define ACK_COUNT_DEFAULT 4

// RGB triplets for the eye colour, default to yellow-red (#bf3f00)
#define EYECOLOUR_RED   0xff // bf
#define EYECOLOUR_GREEN 0x8f // 7f
#define EYECOLOUR_BLUE  0x00

// RGB triplets for the status light colour, default to yellow (#7f3f00)
#define STATUSCOLOUR_RED   0xff
#define STATUSCOLOUR_GREEN 0x8f
#define STATUSCOLOUR_BLUE  0x00

// RGB triplets for the status light colour, default to yellow (#7f7f00)
#define STATUSFLASH_RED   0xff
#define STATUSFLASH_GREEN 0xff
#define STATUSFLASH_BLUE  0x00

// RGB triplets for the blushing colour, default to pink
#define BLUSHCOLOUR_RED   0x7f
#define BLUSHCOLOUR_GREEN 0x00
#define BLUSHCOLOUR_BLUE  0x02

// The Pi does not currently support these as the Unicorn board gets in the way

//#define CYCLE_EYES
//#define STATUS_LIGHTS
//#define VOICE_DETECTOR

// Set the pins for Raspberry pi

#ifdef IMAGES_XERIAN
	#define STARTLED_PIN 29 // GPIO21 (pin 40)
	#define EYEROLL_PIN 23 // GPIO13 (pin 33)
	#define ANNOYED_PIN 27 // GPIO16 (pin 36)
	#define BLUSHING_PIN 25 // GPIO26 (pin 37)
	#define OWO_PIN 0
	#define FAULT_PIN 28 // GPIO20 (pin 38)
#endif

#ifdef IMAGES_ARTHI
	#define STARTLED_PIN 0
	#define EYEROLL_PIN 0
	#define ANNOYED_PIN 27 // GPIO16 (pin 36)
	#define BLUSHING_PIN 25 // GPIO26 (pin 37)
	#define OWO_PIN 29 // GPIO21 (pin 40)
	#define FAULT_PIN 28 // GPIO20 (pin 38)
#endif

#define STATUS_PIN 0
#define VOICE_PIN 0

#define STATUSPIXELS 6

#define ADC_THRESHOLD 768

#endif
