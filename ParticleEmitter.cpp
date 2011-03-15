
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(btVector3* pos, Shader* particleShader, sf::Image* tex, 
								 int width, bool fast, EmitterType ty){
	this->position = pos;
	this->particleShader = particleShader;
	this->particleImage = tex;
	this->spawnParticles();
	this->screenWidth = width;
	this->onlyWhenFast = fast;
	this->type = ty;
	this->age = 0.0;
}
//
//void ParticleEmitter::startExplosion(){
//	curExplosion->time = 0.0;
//	curExplosion->duration = 0.5;
//}
//
//void ParticleEmitter::updateExplosion(float tstep){
//		curShake->time += tstep;
//		if (curShake->time > curShake->duration) {
//			return;
//		}
//		else {
//			float randPos = rand()%10*0.1;
//			if(rand()%2) randPos *= -1.0; //curRot->start.slerp(curRot->end, EASE(t));
//			pos.m_floats[0] = curShake->position.x() + randPos;
//			pos.m_floats[1] = curShake->position.y() + randPos;
//			pos.m_floats[2] = curShake->position.z() + randPos;
//			
//			
//			vector<unsigned> toDelete;
//			for(unsigned i = 0; i < particles.size(); i++){
//				particles[i].updateParticle(tstep, fast);
//				if(particles[i].age > .3) toDelete.push_back(i);
//			}
//			for(unsigned i = 0; i < toDelete.size(); i++) particles.erase(particles.begin()+toDelete[i]);
//			return;
//		}
//}


int ParticleEmitter::chooseZOffset(){
	return 15;
//	switch(this->type){
//		case SMOKE:
//			return 7;
//		case FIRE:
//		case PLASMA:
//		default:
//			return 8;
//	}
}

void ParticleEmitter::randomizeVelocities(float& xvel, float& yvel, float& zvel){
	if(rand()%2) xvel*=-2.0;
	if(rand()%100 > 70) xvel +=0.3;
	if(rand()%100 > 60) xvel -=0.3;
	if(rand()%100 > 67) xvel -=0.4;

	
	if(rand()%4) yvel*=-2.0;
	if(rand()%100 > 70) yvel +=0.3;
	if(rand()%100 > 60) yvel -=0.3;
	if(rand()%100 > 97) yvel -=0.4;

	
	if(rand()%2) zvel*=-2.3;
	if(rand()%100 > 70) zvel +=0.3;
	if(rand()%100 > 60) zvel -=0.3;
	if(rand()%100 > 87) zvel -=0.4;
}
void ParticleEmitter::updateEmitter(float tstep, bool fast){
	age += tstep;
	
	if(type == EXPLOSION){
		for(unsigned i = 0; i < particles.size(); i++){
			particles[i].updateParticle(tstep, fast);
			GLfloat xvel = 1.0;
			GLfloat yvel = 1.0;
			GLfloat zvel = 1.0;
			randomizeVelocities(xvel, yvel, zvel);
			Particle newParticle(btVector3(position->x(),position->y(),position->z()+chooseZOffset()), btVector3(xvel,yvel,zvel));
			if(rand()%100 > 65) particles[i] = newParticle;
		}
		
		return;
	}
	
	
	for(unsigned i = 0; i < particles.size(); i++){
		particles[i].updateParticle(tstep, fast);
		GLfloat xvel = 0.1;
		
		if(rand()%2) xvel*=-1.0;
		if(rand()%100 > 70) xvel +=0.03;
		if(rand()%100 > 60) xvel -=0.03;
		if(rand()%100 > 67) xvel -=0.04;
		GLfloat yvel = 0.1;
		
		if(rand()%2) yvel*=-1.0;
		if(rand()%100 > 70) yvel +=0.03;
		if(rand()%100 > 60) yvel -=0.03;
		if(rand()%100 > 97) yvel -=0.04;
		GLfloat zvel = 0.5;
		
		if(rand()%2) zvel*=-1.3;
		if(rand()%100 > 70) zvel +=0.03;
		if(rand()%100 > 60) zvel -=0.03;
		if(rand()%100 > 87) zvel -=0.04;
		Particle newParticle(btVector3(position->x(),position->y(),position->z()+chooseZOffset()), btVector3(xvel,yvel,zvel));
		if(rand()%100 > 65) particles[i] = newParticle;
	}
	return;
}
void ParticleEmitter::spawnParticles(){
	if(this->type == EXPLOSION){
		for(int i = 0; i < 45; i++){
			Particle newParticle(btVector3(position->x(),position->y(),position->z()), btVector3(2.0,2.0,2.0));
			particles.push_back(newParticle);
		}
		return;
	}
	
	for(int i = 0; i < 20; i++){
		Particle newParticle(btVector3(position->x(),position->y(),position->z()+chooseZOffset()), btVector3(0.0,0.0,0.0));
		particles.push_back(newParticle);
	} 
} 
void ParticleEmitter::renderParticles(bool fast){
	if(!fast && this->onlyWhenFast) return; 
	
	for(unsigned i = 0; i < particles.size(); i++){
		GLint fire = glGetUniformLocation(particleShader->programID(), "fireTex");
		glActiveTexture(GL_TEXTURE0);
		particleImage->Bind();
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glUniform1i(fire, 0);
		
		GLint pos = glGetAttribLocation(particleShader->programID(), "positionIn");
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, 0, sizeof(Particle), &particles[i].position);
		
		GLint base = glGetUniformLocation(particleShader->programID(), "baseSize");
		float value;
		glGetFloatv(GL_POINT_SIZE, &value);
		float size = rand()%20;
		size /= 5.0;
		if(type == EXPLOSION) size*=3.5;
		glUniform1f(base, value*size);
		
		GLint width = glGetUniformLocation(particleShader->programID(), "viewportWidth");
		glUniform1f(width, this->screenWidth);
		
		GLint ag = glGetUniformLocation(particleShader->programID(), "age");
		glUniform1f(ag, this->age);
		
		glDrawArrays(GL_POINTS, 0, 1);
		
		glPopMatrix();
	}
}
