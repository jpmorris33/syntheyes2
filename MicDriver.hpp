#include "inttype.h"
class MicDriver {
	public:
		virtual void init(int pin);
		virtual uint16_t read();
};
