
#include "Camera.h"

#include <cmath>

#define EASE(t) ((t) * (t) * (3.0 - 2.0 * (t)))

#define ZOOM_IN 50.0
#define ZOOM_OUT 70.0

#define DURATION 1.0

using namespace sf;
using namespace std;

Camera::Camera(btVector3 pos) {
	this->pos = pos;
	quat = btQuaternion(btVector3(0, 1, 0), M_PI);
	mousePressed = false;
}

Camera::~Camera() {}

void Camera::update(float tstep) {
	if (curZoom) {
		curZoom->time += tstep;
		if (curZoom->time > curZoom->duration) {
			pos = curZoom->endPos;
			delete curZoom; curZoom = NULL;
		} else {
			float t = curZoom->time / curZoom->duration;
			pos = curZoom->startPos.lerp(curZoom->endPos, EASE(t));
		}
	}
}

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

void Camera::setZoom(btVector3 endPan) {
	delete curZoom;
	curZoom = new Zoom();
	curZoom->startPos = pos;
	curZoom->endPos = endPan;
	curZoom->time = 0.0;
	curZoom->duration = DURATION;
}

void Camera::setTarget(btVector3 target) {
	btScalar yaw =
		atan((pos.x() - target.x()) / (pos.z() - target.z()));
	btScalar pitch =
		atan((pos.y() - target.y()) / (pos.z() - target.z()));
	quat = btQuaternion(btVector3(0, 1, 0), M_PI) * btQuaternion(yaw, pitch, 0);
}

void Camera::handleEvent(sf::Event &event, const sf::Input &input) {
	switch (event.Type) {
		case Event::KeyPressed:
			if (event.Key.Code == Key::Space &&
					(curZoom == NULL || curZoom->endPos.z() == ZOOM_IN)) {
				setZoom(btVector3(0, 0, ZOOM_OUT));
			}
			break;
		case Event::KeyReleased:
			setZoom(btVector3(0, 0, ZOOM_IN));
			break;
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
