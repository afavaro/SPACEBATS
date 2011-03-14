#ifndef MODEL_H
#define MODEL_H

#include "Framework.h"
#include "Shader.h"
#include "Framebuffer.h"

#include "btBulletDynamicsCommon.h"

enum RenderPass {
	NORMALS_PASS,
	FINAL_PASS,
};

class Model {
	public:
		Model();
		~Model();

		void loadFromFile(const std::string &dir, const std::string &filename, Assimp::Importer &importer);
		void useShader(Shader *shader);

		void setTransformation(btTransform &t);

		void render(RenderPass pass);
	
		void setScaleFactor(float f);

		static sf::Image white;

		static Shader *normalShader, *toonShader;
		static void loadShaders();

		static Framebuffer *normalsBuffer;
		static void setNormalsBuffer(Framebuffer *fb);

	btTransform transformation;
	
	const aiScene *scene;
	float scaleFactor;
	private:
//		btTransform transformation;
//		
//		const aiScene *scene;
//		float scaleFactor;

		sf::Image *diffuse, *specular;

		void setMeshData(aiMesh *mesh, Shader *shader, RenderPass pass);

		unsigned *indexBuffer;
};

#endif
