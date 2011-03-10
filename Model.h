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

		void render(RenderPass pass, Framebuffer *normalsBuffer);

		static sf::Image white;

	private:
		btTransform transformation;
		const aiScene *scene;

		Shader *shader;
		sf::Image *diffuse, *specular;

		void setMeshData(aiMesh *mesh, Shader *shader, RenderPass pass);

		unsigned *indexBuffer;
};

#endif
