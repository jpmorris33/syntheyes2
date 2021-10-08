#include "../MicDriver.hpp"

class PicoMic : public MicDriver {
	public:
		void init(int pin);
		uint16_t read();
};
