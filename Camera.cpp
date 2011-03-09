
#include "Camera.h"

using namespace sf;

Camera::Camera(aiVector3D pos, aiMatrix3x3 basis) {
	this->pos = pos;
	this->basis = basis;
	mousePressed = false;
}

Camera::~Camera() {}

void Camera::setProjectionAndView(float aspectRatio) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			pos.x + basis.a3, pos.y + basis.b3, pos.z + basis.c3,
			basis.a2, basis.b2, basis.c2);
}	

void Camera::handleEvent(sf::Event &event) {
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
