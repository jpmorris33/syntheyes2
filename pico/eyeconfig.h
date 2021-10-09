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

//#define CYCLE_EYES
#define STATUS_LIGHTS
#define VOICE_DETECTOR

// Set the pins for Raspberry Pi pico
#define STARTLED_PIN 16
#define EYEROLL_PIN 17
#define ANNOYED_PIN 18
#define BLUSHING_PIN 19
#define OWO_PIN 0
#define FAULT_PIN 0
#define STATUS_PIN 15
#define VOICE_PIN 26

#define STATUSPIXELS 6

#define ADC_THRESHOLD 768

#endif