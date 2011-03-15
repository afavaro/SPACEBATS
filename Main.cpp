
#include "Framework.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "InputListener.h"
#include "Framebuffer.h"
#include "MotionBlur.h"
#include "Ship.h"
#include "BodyEmitter.h"
#include "HUD.h"
#include "ParticleEngine.h"
#include "StatusBar.h"
#include "Gate.h"
#include "LevelManager.h"
#include "Level.h"
#include "StatusText.h"

#include <btBulletDynamicsCommon.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include "MusicManager.h"

#define TIMESTEP (1.0 / 60.0)

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
GLfloat timeElapsed = 0.0;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer;
Shader *blurShader;

sf::Image background, splash;

ParticleEngine pEngine;

Camera camera;

BodyEmitter *bodyEmitter;

btDiscreteDynamicsWorld *world;

vector<InputListener*> inputListeners;

Framebuffer *normalsBuffer = NULL;

const int NUM_MOTION_BLUR_FRAMES = 4;
MotionBlur* motionBlur;
bool useMotionBlur = false;

bool isStarted = false;

HUD* hud;
StatusBar* boostbar;
StatusBar* healthbar;
StatusText *statusText;

MusicManager music;
LevelManager levels(3);

Ship spaceship(btVector3(0.0, 0.0, 0.0), &camera, &pEngine, &levels);

void initOpenGL();
void loadAssets();
void handleInput();
void renderFrame();
void renderSplash();

btVector3 flame(0.0,0.0,0.0);

int main(int argc, char** argv) {
	
	srand(time(0));
	
	initOpenGL();
	
	btBroadphaseInterface *broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration *collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfig);
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, 0, 0));
	
	spaceship.setWorld(world);

	statusText = new StatusText(&window, btVector4(-1.0, -0.73, 0.5, 0.25));

	loadAssets();
	
	hud = new HUD(&spaceship);
	boostbar = new StatusBar(btVector4(0.85, -0.95, 0.05, 1.0));
	healthbar = new StatusBar(btVector4(0.92, -0.95, 0.05, 1.0));
	healthbar->setTopColor(btVector4(0, 1, 0, 0.5));
	healthbar->setBottomColor(btVector4(1, 0, 0, 0.5));
	
	hud->addComponent(boostbar);
	hud->addComponent(healthbar);
	hud->addComponent(statusText);

	
	Gate::loadChangeImage();
	
	spaceship.setBoostBar(boostbar);
	spaceship.setHealthBar(healthbar);
	spaceship.setStatusText(statusText);
	
	motionBlur = new MotionBlur(NUM_MOTION_BLUR_FRAMES, window.GetWidth(), window.GetHeight());
	glClear(GL_ACCUM_BUFFER_BIT);
	
	inputListeners.push_back(&camera);
	inputListeners.push_back(&spaceship);
	inputListeners.push_back(&levels);
	
	pEngine.setWindow(window.GetWidth());
	pEngine.addEmitter(&spaceship.pos, FIRE, false);
	pEngine.addEmitter(&spaceship.pos, SMOKE, false);
	pEngine.addEmitter(&spaceship.pos, PLASMA, true);
	
	music.playSound(BACKGROUND);
	
	int counter = 0;

	//renderSplash();
	
	// Put your game loop here (i.e., render with OpenGL, update animation)
	while (window.IsOpened()) {	
		handleInput();

		
		
//		if (!isStarted) {
//			window.Display();
//			continue;
//		}
		
		if(levels.shouldShowSplashScreen()){
			levels.renderSplash();
			window.Display();
			continue;
		}
		
		float elapsed = clck.GetElapsedTime();
		accum += elapsed;
		timeElapsed += elapsed;
		
		counter++;
		
		if(counter % 20 == 0)
			printf("Elapsed time: %f\n", timeElapsed);
		clck.Reset();
		while (accum > TIMESTEP) {
			spaceship.update(TIMESTEP);
			world->stepSimulation(TIMESTEP);

			spaceship.testCollision();
			pEngine.updateEmitters(TIMESTEP, useMotionBlur);

			
			/// if the elapsed time is greater than the first landmark on the level
			/// pop it and emit that kind of body
			if(levels.current()->shouldEmitLandmark(timeElapsed)){
				printf("should emit body\n");
				BodyType landmarkType = levels.current()->firstLandmark();
				bodyEmitter->emit(landmarkType);
			}
			
			bodyEmitter->emitBodies(TIMESTEP, levels.current());
			camera.update(TIMESTEP);
			accum -= TIMESTEP;
		}
				
		renderFrame();
		window.Display();
	}
	
	delete blurShader;
	delete normalsBuffer;
	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	
	delete motionBlur;
	delete boostbar;
	delete healthbar;
	delete hud;
	
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
	blurShader = new Shader("shaders/blur");
	StatusBar::loadShader();
	
	Level::loadShaders();
	
	bodyEmitter = new BodyEmitter(world);
	bodyEmitter->loadModels();
	
	Model::loadShaders();
	normalsBuffer = new Framebuffer(window.GetWidth(), window.GetHeight());
	Model::setNormalsBuffer(normalsBuffer);
	
	background.LoadFromFile("models/space2.jpg");
	splash.LoadFromFile("models/TitleScreen.jpg");
	
	spaceship.model.loadFromFile("models/ship", "space_frigate_0.3DS", importer);
	spaceship.model.setScaleFactor(0.9);

	statusText->loadFont("fonts/Spaceship Bullet.ttf");
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
			case sf::Event::KeyPressed:
				switch (evt.Key.Code) {
					case sf::Key::Escape:
						window.Close();
						break;
					case sf::Key::Space:
						if (levels.shouldShowSplashScreen()) {
							levels.setSplash(false);
							if(levels.currentLevel == 0){
								levels.nextLevel();
							}
							clck.Reset();
							return;
						}

						if(boostbar->getValue() <= 0) return;

						if (useMotionBlur) return;
						
						music.playSound(POWERUP);
						useMotionBlur = true;
						bodyEmitter->setBoostMode(true);
						bodyEmitter->boostSpeed();
						break;
					default:
						break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (evt.Key.Code) {
					case sf::Key::Space:
						
						music.stopSound(POWERUP);
						useMotionBlur = false;
						bodyEmitter->setBoostMode(false);
						bodyEmitter->resetSpeed();
						break;
					default:
						break;
				}
				break;
			default: 
				break;
		}
		for (unsigned i = 0; i < inputListeners.size(); i++)
			inputListeners[i]->handleEvent(evt, window.GetInput());
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

void clearNormalsBuffer()
{
	normalsBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	normalsBuffer->unbind();
}

void renderSplash() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(Level::bgShader->programID());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLint tex = glGetUniformLocation(Level::bgShader->programID(), "texture");
	glUniform1i(tex, 0);
	glActiveTexture(GL_TEXTURE0);
	splash.Bind();

	GLint pos = glGetAttribLocation(Level::bgShader->programID(), "positionIn");
	glBegin(GL_QUADS);
	glVertexAttrib2f(pos, 1.0, -1.0);
	glVertexAttrib2f(pos, 1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, 1.0);
	glVertexAttrib2f(pos, -1.0, -1.0);
	glEnd();
}

void renderFrame() {
	glViewport(0, 0, window.GetWidth(), window.GetHeight());

	clearNormalsBuffer();
	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());
	spaceship.model.render(NORMALS_PASS);
	bodyEmitter->drawBodies(NORMALS_PASS);
	
	if (motionBlur->shouldRenderFrame()) {	
		motionBlur->bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		levels.current()->renderBackground();

		glClear(GL_DEPTH_BUFFER_BIT);

		camera.setProjectionAndView((float)window.GetWidth() / window.GetHeight());
		setupLights();

		bodyEmitter->drawBodies(FINAL_PASS);
		motionBlur->unbind();
	}	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(useMotionBlur && boostbar->getValue() <= 0){
		useMotionBlur = false;
		bodyEmitter->setBoostMode(false);
		bodyEmitter->resetSpeed();
	}
	
	if(useMotionBlur){
		motionBlur->render(blurShader);
	} else {
		levels.current()->renderBackground();
	}
	motionBlur->update();
	
	glClear(GL_DEPTH_BUFFER_BIT);	


	
	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());
	setupLights();
	bodyEmitter->drawBodies(FINAL_PASS);
	glSecondaryColor3f(0.0,0.0,0.0);
	spaceship.model.render(FINAL_PASS);
	//cout << "Ship at: " << spaceship.pos.x() << "::" << spaceship.pos.y() << "::" << spaceship.pos.z() << endl;
	camera.setProjectionAndView((float)window.GetWidth()/window.GetHeight());
	pEngine.renderEmitters(useMotionBlur);

	hud->render();	
}
