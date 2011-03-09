
#include "Camera.h"

using namespace sf;

Camera::Camera(aiVector3D pos, aiMatrix3x3 basis) {
	this->pos = pos;
	this->basis = basis;
	mousePressed = false;
}

Camera::~Camera() {}

GLfloat Camera::quadEaseIn(GLfloat timePassed, GLfloat power){
	return pow(timePassed, power);
}

GLfloat Camera::quadEaseOut(GLfloat timePassed, GLfloat power){
	return 1- quadEaseIn(1.0 - timePassed, power);
}

GLfloat Camera::quadEaseInOut(GLfloat timePassed, GLfloat power){
	if(timePassed < 0.5) return quadEaseIn(2.0*timePassed, power)/2.0;
	return quadEaseOut(2.0*(timePassed-0.5),power)/2.0 + 0.5;
}

void Camera::easeToNewPosition(){
	if(time == duration) return;
	pos = change*quadEaseInOut(time/duration, 2.0) + startVal;
	time++;
}

void Camera::setProjectionAndView(float aspectRatio) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			pos.x + basis.a3, pos.y + basis.b3, pos.z + basis.c3,
			basis.a2, basis.b2, basis.c2);
	easeToNewPosition();
}	

void Camera::pull(aiVector3D planePosition, aiVector3D increment){
	time = 0;
	startVal = pos;
	pos+=increment;
	change = increment;
	easeToNewPosition();
}

void Camera::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case Event::MouseButtonPressed:
			if (event.MouseButton.Button == Mouse::Left) {
				mousePressed = true;
				mouseX = event.MouseButton.X;
				mouseY = event.MouseButton.Y;
			}
			break;
		case Event::MouseButtonReleased:
			if (event.MouseButton.Button == Mouse::Left)
				mousePressed = false;
			break;
		case Event::MouseMoved:
			if (mousePressed) {
				float tx = 0.01 * (event.MouseMove.X - mouseX);

				aiMatrix3x3 posMatX, basisMatX;				
				aiMatrix3x3::Rotation(tx, aiVector3D(basis.a2, basis.b2, basis.c2), basisMatX);
				aiMatrix3x3::Rotation(-tx, aiVector3D(basis.a2, basis.b2, basis.c2), posMatX);
				pos *= posMatX;
				basis *= basisMatX;

				mouseX = event.MouseMove.X;
				mouseY = event.MouseMove.Y;
			}
			break;
		default:
			break;
	}
}
