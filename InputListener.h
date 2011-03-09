#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "Framework.h"

class InputListener {
	public:
		virtual void handleEvent(sf::Event &event) = 0;
};

#endif
