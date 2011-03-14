#ifndef STATUS_TEXT_H
#define STATUS_TEXT_H

#include "Framework.h"
#include "HUDComponent.h"

#include "btBulletDynamicsCommon.h"

class StatusText : public HUDComponent {
	public:
		StatusText(sf::RenderWindow *window, btVector4 rect);
		~StatusText();

		void loadFont(const std::string &filename);
		
		void render();

		void addScore(int points);
		void updateTime(float tstep);

	private:
		int score;
		float time;

		btVector4 rect;

		sf::Font font;
		sf::RenderWindow *window;
};

#endif
