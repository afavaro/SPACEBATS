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

		void handleEvent(sf::Event &event);

	private:
		aiVector3D pos;
		aiMatrix3x3 basis;
		
		bool mousePressed;
		int mouseX, mouseY;
};

#endif
