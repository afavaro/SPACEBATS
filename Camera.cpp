
#include "Camera.h"

#include <cmath>

using namespace sf;

Camera::Camera(btVector3 pos) {
	this->pos = pos;
	quat = btQuaternion(btVector3(0, 1, 0), M_PI);
	mousePressed = false;
}

Camera::~Camera() {}

void Camera::setProjectionAndView(float aspectRatio) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	btMatrix3x3 basis(quat);
	gluLookAt(pos.x(), pos.y(), pos.z(),
			pos.x() + basis[0].z(), pos.y() + basis[1].z(), pos.z() + basis[2].z(),
			basis[0].y(), basis[1].y(), basis[2].y());
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

				btMatrix3x3 posMatX;
				posMatX.setEulerYPR(tx, 0.0, 0.0);

				pos = posMatX * pos;

				mouseX = event.MouseMove.X;
				mouseY = event.MouseMove.Y;
			}
			break;
		default:
			break;
	}
}
