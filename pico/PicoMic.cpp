#include "hardware/adc.h"
#include "PicoMic.hpp"

void PicoMic::init(int pin) {
	adc_init();
	adc_gpio_init(pin);
	adc_select_input(0);
}

uint16_t PicoMic::read() {
	return adc_read();
}
