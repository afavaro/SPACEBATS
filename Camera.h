#ifndef CAMERA_H
#define CAMERA_H

#include "Framework.h"
#include "InputListener.h"

#define Z_NEAR 0.1
#define Z_FAR 200.0

class Camera : public InputListener {
public:
	Camera(aiVector3D pos, aiMatrix3x3 basis);
	~Camera();

	void setProjectionAndView(float aspectRatio);
	GLfloat quadEaseIn(GLfloat timePassed, GLfloat power);
	GLfloat quadEaseOut(GLfloat timePassed, GLfloat power);
	GLfloat quadEaseInOut(GLfloat timePassed, GLfloat power);
	void easeToNewPosition();
	void pull(aiVector3D planePosition, aiVector3D newPosition);
	void handleEvent(sf::Event &event, const sf::Input &input);
	
private:
	aiVector3D pos;
	aiMatrix3x3 basis;
	
	GLfloat time, duration; 
	aiVector3D change, startVal;
	
	bool mousePressed;
	int mouseX, mouseY;
};

#endif
