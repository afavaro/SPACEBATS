#ifndef CAMERA_H
#define CAMERA_H

#include "Framework.h"
#include "InputListener.h"

#include "btBulletDynamicsCommon.h"

#define Z_NEAR 0.1
#define Z_FAR 300.0

class Camera : public InputListener {
public:
	Camera(btVector3 pos, btMatrix3x3 basis);
	~Camera();

	void setProjectionAndView(float aspectRatio);
	void handleEvent(sf::Event &event, const sf::Input &input);
	
private:
	btVector3 pos;
	btMatrix3x3 basis;
		
	bool mousePressed;
	int mouseX, mouseY;
};

#endif
