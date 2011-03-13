#ifndef CAMERA_H
#define CAMERA_H

#include "Framework.h"
#include "InputListener.h"

#include "btBulletDynamicsCommon.h"

#define Z_NEAR 0.1
#define Z_FAR 300.0

struct Zoom {
	btVector3 startPos, endPos;
	btScalar time, duration;
};

class Camera : public InputListener {
public:

	Camera(btVector3 pos);
	~Camera();

	void setProjectionAndView(float aspectRatio);

	void handleEvent(sf::Event &event, const sf::Input &input);

	void update(float tstep);

	void setTarget(btVector3 target);
	
private:
	void setZoom(btVector3 endPos);

	btVector3 pos;
	btQuaternion quat;

	Zoom *curZoom;
		
	bool mousePressed;
	int mouseX, mouseY;
};

#endif
