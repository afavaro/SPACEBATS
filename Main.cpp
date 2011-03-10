
#include "Framework.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "InputListener.h"
#include "Framebuffer.h"
#include "MotionBlur.h"
#include "Ship.h"

#include <btBulletDynamicsCommon.h>

#include <cmath>

#define TIMESTEP 0.01

using namespace std;

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::RenderWindow window(sf::VideoMode(1000, 650), "sPaCEbaTS", sf::Style::Close, settings);

// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

GLfloat accum = 0.0;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer, marsImporter;
Shader *phongShader, *blurShader;

sf::Image background;

Model mars;

Camera camera(
			  btVector3(0.0, 0.0, 50.0),
			  btMatrix3x3(
						  1.0, 0.0, 0.0,
						  0.0, 1.0, 0.0,
						  0.0, 0.0, -1.0));

Ship spaceship(btVector3(0.0, 0.0, 0.0), &camera);

vector<InputListener*> inputListeners;

Framebuffer *normalsBuffer = NULL;

static int frameCounter = 0;
const int NUM_MOTION_BLUR_FRAMES = 4;
MotionBlur* motionBlur;

void initOpenGL();
void loadAssets();
void handleInput();
void renderFrame();
void renderBackground();

int main(int argc, char** argv) {
	
    initOpenGL();
    loadAssets();
	motionBlur = new MotionBlur(NUM_MOTION_BLUR_FRAMES, window.GetWidth(), window.GetHeight());
	glClear(GL_ACCUM_BUFFER_BIT);
	
	inputListeners.push_back(&camera);
	inputListeners.push_back(&spaceship);
	
	
    // Put your game loop here (i.e., render with OpenGL, update animation)
    while (window.IsOpened()) {	
        handleInput();

				accum += clck.GetElapsedTime();
				clck.Reset();
				while (accum > TIMESTEP) {
					spaceship.update(TIMESTEP);
					accum -= TIMESTEP;
				}

        renderFrame();
        window.Display();
    }
	
	delete phongShader;
	delete blurShader;
	
	delete normalsBuffer;
	
    return 0;
}



void initOpenGL() {
    // Initialize GLEW on Windows, to make sure that OpenGL 2.0 is loaded
#ifdef FRAMEWORK_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        std::cerr << glewGetErrorString(error) << std::endl;
        exit(-1);
    }
    if (!GLEW_VERSION_2_0 || !GL_EXT_framebuffer_object) {
        std::cerr << "This program requires OpenGL 2.0 and FBOs" << std::endl;
        exit(-1);
    }
#endif
	
    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
}

void loadAssets() {
	phongShader = new Shader("shaders/phong");
	blurShader = new Shader("shaders/blur");

	Model::loadShaders();
	normalsBuffer = new Framebuffer(window.GetWidth(), window.GetHeight());
	Model::setNormalsBuffer(normalsBuffer);

//	background.LoadFromFile("models/Space-Background.jpg");
//	spaceship.model.loadFromFile("models/ship", "space_frigate_0.3DS", importer);
//	mars.loadFromFile("models", "mars.3ds", marsImporter);
//	aiMatrix4x4 rot;
//	aiMatrix4x4::RotationX(-M_PI / 2.0, rot);
//	spaceship.setTransformation(rot);
	

	spaceship.model.loadFromFile("models/ship", "space_frigate_0.3DS", importer);

}




void handleInput() {
    //////////////////////////////////////////////////////////////////////////
    // TODO: ADD YOUR INPUT HANDLING HERE. 
    //////////////////////////////////////////////////////////////////////////
	
    // Event loop, for processing user input, etc.  For more info, see:
    // http://www.sfml-dev.org/tutorials/1.6/window-events.php
    sf::Event evt;
    while (window.GetEvent(evt)) {
        switch (evt.Type) {
			case sf::Event::Closed: 
				// Close the window.  This will cause the game loop to exit,
				// because the IsOpened() function will no longer return true.
				window.Close(); 
				break;
			case sf::Event::Resized: 
				// If the window is resized, then we need to change the perspective
				// transformation and viewport
				glViewport(0, 0, evt.Size.Width, evt.Size.Height);
				break;
			default: 
				for (unsigned i = 0; i < inputListeners.size(); i++)
					inputListeners[i]->handleEvent(evt, window.GetInput());
				break;
        }
    }
}


void setupLights()
{
	GLfloat pos[] = { 0.0, 1.0, 0.0, 0.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}

void renderBackground()
{
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glActiveTexture(GL_TEXTURE0);
	background.Bind();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glEnd();
}

void renderObjects(RenderPass pass) {
	for(int i = 0; i < 10; i++){
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(10, 0, -100* i);
		glTranslatef(0, 0, frameCounter / 5.0);
		mars.render(pass);
		glPopMatrix();
	}
	
	for(int i = 0; i < 10; i++){
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(-10, -10, -100* i);
		glTranslatef(0, 0, frameCounter / 5.0);
		mars.render(pass);
		glPopMatrix();
	}
	
}

void renderFrame() {
	frameCounter++;
	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());
	
	//Render normals
	normalsBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	normalsBuffer->unbind();

	spaceship.model.render(NORMALS_PASS);

	
	/*
	//Render this frame to motion blur
	motionBlur->bind();
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLights();
	spaceship.model.useShader(toonShader);
	spaceship.model.render(FINAL_PASS, normalsBuffer);
	motionBlur->unbind();
	
	//int frames = frameCounter < NUM_MOTION_BLUR_FRAMES ? frameCounter : NUM_MOTION_BLUR_FRAMES;
	//cout << frames << " frames" << endl;
	//float val = 1.0 / frames;
	//cout << "val per" << val << endl;
	*/
	//int frames = frameCounter < NUM_MOTION_BLUR_FRAMES ? frameCounter : NUM_MOTION_BLUR_FRAMES;
	//float val = 1.0 / frames;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderBackground();

	glClear(GL_DEPTH_BUFFER_BIT);

	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());

	setupLights();
//	motionBlur->render(blurShader);
	
	spaceship.model.render(FINAL_PASS);
}
