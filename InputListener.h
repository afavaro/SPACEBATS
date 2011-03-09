#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "Framework.h"

class InputListener {
	public:
		virtual void handleEvent(sf::Event &event, const sf::Input &input) = 0;
};

#endif
