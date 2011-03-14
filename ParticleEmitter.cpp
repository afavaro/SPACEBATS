
#include "ParticleEmitter.h"

//ParticleEmitter::ParticleEmitter(Ship* ship, Shader* particleShader, sf::Image* tex){
ParticleEmitter::ParticleEmitter(btVector3* pos, Shader* particleShader, sf::Image* tex, int width){
	this->position = pos;
	this->particleShader = particleShader;
	this->particleImage = tex;
	//this->spaceship = ship;
	this->spawnParticles();
	this->screenWidth = width;
}

void ParticleEmitter::updateEmitter(float tstep, bool fast){
	for(unsigned i = 0; i < particles.size(); i++){
		particles[i].updateParticle(tstep, fast);
		//btVector3 div = *position/10000.0;
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
		
		if(rand()%100 > 70) zvel +=0.03;
		if(rand()%100 > 60) zvel -=0.03;
		if(rand()%100 > 87) zvel -=0.04;
		Particle newParticle(btVector3(position->x(),position->y(),position->z()-42), btVector3(xvel,yvel,zvel));
		if(rand()%100 > 65) particles[i] = newParticle;
	}
	return;
	//spawnParticles();
	for(unsigned i = 0; i < particles.size(); i++){
		//particles[i].updateParticle(tstep);
		btVector3 newPos(0,0,0);
//		switch(emitType){
//			case 0:
//				if(rand()%100 > 70) newPos.x += 0.13;
//				if(rand()%100 > 70) newPos.x -= 0.13;
//				if(rand()%100 > 60) newPos.z += 0.15;
//				if(rand()%100 > 60) newPos.z -= 0.15;
//				if(rand()%100 > 60) newPos.y -= 0.05;
//				if(rand()%100 > 90) newPos.y += 0.12;
//				break;
//			case 1:
				//if(rand()%100 > 90) newPos.m_floats[0] += 0.13;
//				if(rand()%100 > 90) newPos.m_floats[0] += 0.03;
//				if(rand()%100 > 90) newPos.m_floats[0] -= 0.063;
//				if(rand()%100 > 90) newPos.m_floats[0] -= 0.13;
//				if(rand()%100 > 50) newPos.m_floats[2] += 0.15;
//				if(rand()%100 > 40) newPos.m_floats[2] -= 0.15;
//				if(rand()%100 > 40) newPos.m_floats[2] += 0.022;
//				if(rand()%100 > 30) newPos.m_floats[2] -= 0.015;
//				if(rand()%100 > 60) newPos.m_floats[1] -= 0.05;
//				if(rand()%100 > 90) newPos.m_floats[1] += 0.12;
//				break;
//		}
//		GLfloat xvel = 0.001;
//		if(rand()%100 > 70) xvel +=0.003;
//		if(rand()%100 > 60) xvel -=0.003;
//		if(rand()%100 > 67) xvel -=0.004;
//		GLfloat yvel = 0.001;
//		if(rand()%100 > 70) yvel +=0.003;
//		if(rand()%100 > 60) yvel -=0.003;
//		if(rand()%100 > 97) yvel -=0.004;
//		GLfloat zvel = 0.005;
//		if(rand()%100 > 70) zvel +=0.003;
//		if(rand()%100 > 60) zvel -=0.003;
//		if(rand()%100 > 87) zvel -=0.004;
		
		GLfloat xvel = 0.00;
		GLfloat yvel = 0.00;
		GLfloat zvel = 0.00;
		Particle newParticle(newPos, btVector3(xvel,yvel,zvel));
		if(rand()%100 > 65) particles[i] = newParticle;
	}
}
void ParticleEmitter::spawnParticles(){
	//aiMesh *mesh = spaceship->GetMesh();
	for(int i = 0; i < 20; i++){
		//Particle newParticle(btVector3(mesh->mVertices[0].x,mesh->mVertices[0].y,mesh->mVertices[0].z), btVector3(0.0,0.0,0.0));
		Particle newParticle(btVector3(position->x(),position->y(),position->z()-42), btVector3(0.0,0.0,0.0));
		particles.push_back(newParticle);
	} 
} 
void ParticleEmitter::renderParticles(){
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
	
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glPushMatrix();
//	
//	GLfloat glmat[16];
//	btTransform transform(btQuaternion(0, 0, 0, 1), btVector3(position->x(),position->y(),position->z()-47));
//	transform.getOpenGLMatrix(glmat);
//	//spaceship->model.transformation.getOpenGLMatrix(glmat);
//	glMultMatrixf(glmat);
//	
//	glScalef(.5,.5,.5);
//	
//	cout << "Emitter at: " << position->x() << "::" << position->y() << "::" << position->z()<<endl;
//
//	//glScalef(spaceship->model.scaleFactor, spaceship->model.scaleFactor, spaceship->model.scaleFactor);
//	
//	GLint fire = glGetUniformLocation(particleShader->programID(), "fireTex");
//	glActiveTexture(GL_TEXTURE0);
//	particleImage->Bind();
//	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
//	glUniform1i(fire, 0);
//	
//	GLint pos = glGetAttribLocation(particleShader->programID(), "positionIn");
//	glEnableVertexAttribArray(pos);
//	glVertexAttribPointer(pos, 3, GL_FLOAT, 0, sizeof(Particle), &particles[0].position);
//	
//	GLint base = glGetUniformLocation(particleShader->programID(), "baseSize");
//	float value;
//	glGetFloatv(GL_POINT_SIZE, &value);
//	glUniform1f(base, value);
//	
//	GLint width = glGetUniformLocation(particleShader->programID(), "viewportWidth");
//	glUniform1f(width, this->screenWidth);
//	
////	GLint alph = glGetUniformLocation(particleShader, "alpha");
////	float alpha = -999.0;
////	glUniform1f(alph, alpha);
//	
//	glDrawArrays(GL_POINTS, 0, particles.size());
//	
//	glPopMatrix();
	
	
	
	for(unsigned i = 0; i < particles.size(); i++){
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		
		GLfloat glmat[16];
		btTransform transform(btQuaternion(0, 0, 0, 1), 
							  btVector3(particles[i].position.x(),particles[i].position.y(),particles[i].position.z()));
		transform.getOpenGLMatrix(glmat);
		//spaceship->model.transformation.getOpenGLMatrix(glmat);
		glMultMatrixf(glmat);
		
		glScalef(.01,.01,.01);
		
		GLint fire = glGetUniformLocation(particleShader->programID(), "fireTex");
		glActiveTexture(GL_TEXTURE0);
		particleImage->Bind();
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glUniform1i(fire, 0); // The fire map will be GL_TEXTURE0
		
		GLint pos = glGetAttribLocation(particleShader->programID(), "positionIn");
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, 0, sizeof(Particle), &particles[i].position);
		
		GLint base = glGetUniformLocation(particleShader->programID(), "baseSize");
		float value;
		glGetFloatv(GL_POINT_SIZE, &value);
		float size = rand()%20;
		size /= 10.0;
		glUniform1f(base, value*size);
		
		GLint width = glGetUniformLocation(particleShader->programID(), "viewportWidth");
		glUniform1f(width, this->screenWidth);
		
		glDrawArrays(GL_POINTS, 0, 1);
		
		glPopMatrix();
	}
}
