
#include "ParticleEngine.h"

ParticleEngine::ParticleEngine(){
	sf::Image* fiya = new sf::Image();
	fiya->Bind();
	fiya->LoadFromFile("models/FireRed.png");
	particleImages.push_back(fiya);
	
	sf::Image* smoke = new sf::Image();
	smoke->Bind();
	smoke->LoadFromFile("models/Smoke.png");
	particleImages.push_back(smoke);
	
	sf::Image* electric = new sf::Image();
	electric->Bind();
	electric->LoadFromFile("models/SparkBlue.png");
	particleImages.push_back(electric);	
	
	sf::Image* fiy = new sf::Image();
	fiy->Bind();
	fiy->LoadFromFile("models/FireRed.png");
	particleImages.push_back(fiy);

	Shader* fireShader = new Shader("shaders/fire");
	if (!fireShader->loaded()) {
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << fireShader->errors() << std::endl;
		exit(-1);
	}
	particleShaders.push_back(fireShader);
}

void ParticleEngine::setWindow(int width){
	this->width = width;
}

//void ParticleEngine::addEmitter(Ship* ship, int particleType){
void ParticleEngine::addEmitter(btVector3* pos, EmitterType type, bool onlyActiveWhenFast, 
								bool timed, int imageIndex){
	
	if(imageIndex == -1) imageIndex = type;
	ParticleEmitter newEmitter(pos, particleShaders[0], particleImages[imageIndex], 
							   this->width, onlyActiveWhenFast, type);
	emitters.push_back(newEmitter);
}

void ParticleEngine::updateEmitters(double tstep, bool fast){
	for(unsigned i = 0; i < emitters.size(); i++){
		emitters[i].updateEmitter(tstep, fast);
		if(emitters[i].type == EXPLOSION && emitters[i].age > 1.0){
			emitters.erase(emitters.begin()+i);
			i--;
		}
	}
	
}

void ParticleEngine::renderEmitters(bool fast){
	
	glUseProgram(particleShaders[0]->programID());
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_POINT_SPRITE);
	
	for(unsigned i = 0; i < emitters.size(); i++){
		emitters[i].renderParticles(fast);
	}
	
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}