
#include "Model.h"
#include "Camera.h"

#include <sys/stat.h>

using namespace std;
using namespace sf;

Image Model::white(1, 1, sf::Color(255, 255, 255));

Model::Model() {
	scene = NULL;
	shader = NULL;
	diffuse = specular = NULL;
	indexBuffer = NULL;
	transformation = btTransform::getIdentity();
}

Model::~Model() {
	delete diffuse;
	delete specular;
	delete[] indexBuffer;
}

void Model::loadFromFile(const string &dir, const string &filename, Assimp::Importer &importer) {
	scene = importer.ReadFile(dir + "/" + filename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcessPreset_TargetRealtime_Quality);

	if (!scene || scene->mNumMeshes <= 0) {
		cerr << importer.GetErrorString() << endl;
		return;
	}

	aiMesh *mesh = scene->mMeshes[0];
	if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
		cout << "Skipping non triangle mesh" << endl;
		return;
	}

	indexBuffer = new unsigned[mesh->mNumFaces * 3];

	for (unsigned i = 0; i < mesh->mNumFaces; i++) {
		for (unsigned j = 0; j < 3; j++) {
			indexBuffer[3 * i + j] = mesh->mFaces[i].mIndices[j];
		}
	}

	struct stat info;
	string dpath = dir + "/diffuse.jpg";
	string spath = dir + "/specular.jpg";

	if (stat(dpath.c_str(), &info) == 0) {
		diffuse = new Image();
		diffuse->LoadFromFile(dpath);
		diffuse->Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	} else
		cout << "No diffuse texture in " << dir << endl;

	if (stat(spath.c_str(), &info) == 0) {
		specular = new Image();
		specular->LoadFromFile(spath);
		specular->Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	} else
		cout << "No specular texture in " << dir << endl;
}

void Model::useShader(Shader *shader) {
	this->shader = shader;
}

void Model::setTransformation(btTransform &t) {
	transformation = t;
}

static void setMaterial(aiMaterial *mat, Shader *shader)
{
	aiColor3D color;

	GLint diffuse = glGetUniformLocation(shader->programID(), "Kd");
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	glUniform3f(diffuse, color.r, color.g, color.b);

	GLint specular = glGetUniformLocation(shader->programID(), "Ks");
	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	glUniform3f(specular, color.r, color.g, color.b);

	GLint ambient = glGetUniformLocation(shader->programID(), "Ka");
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	glUniform3f(ambient, color.r, color.g, color.b);

	GLint shininess = glGetUniformLocation(shader->programID(), "alpha");
	float value;
	if (mat->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
		glUniform1f(shininess, value);
	else
		glUniform1f(shininess, 1.0f);

}

static void setTextures(Image *diffuse, Image *specular, Shader *shader, Framebuffer *normalsBuffer)
{
	GLint diff = glGetUniformLocation(shader->programID(), "diffuseMap");
	glUniform1i(diff, 1);
	glActiveTexture(GL_TEXTURE1);
	if (diffuse) diffuse->Bind();
	else Model::white.Bind();

	GLint spec = glGetUniformLocation(shader->programID(), "specularMap");
	glUniform1i(spec, 2);
	glActiveTexture(GL_TEXTURE2);
	if (specular) specular->Bind();
	else Model::white.Bind();

	GLint norm = glGetUniformLocation(shader->programID(), "normalMap");
	glUniform1i(norm, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalsBuffer->colorTextureId());
}

void Model::setMeshData(aiMesh *mesh, Shader *shader, RenderPass pass)
{
	GLint pos = glGetAttribLocation(shader->programID(), "positionIn");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mVertices);

	GLint normal = glGetAttribLocation(shader->programID(), "normalIn");
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mNormals);

	if (pass == FINAL_PASS) {
		if (diffuse) {
			GLint texcoord = glGetAttribLocation(shader->programID(), "texcoordIn");
			glEnableVertexAttribArray(texcoord);
			glVertexAttribPointer(texcoord, 2, GL_FLOAT, 0, sizeof(aiVector3D), mesh->mTextureCoords[0]);
		}
	}
}

void Model::render(RenderPass pass, Framebuffer *normalsBuffer) {
	glUseProgram(shader->programID());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	GLfloat glmat[16];
	transformation.getOpenGLMatrix(glmat);
	glMultMatrixf(glmat);

	aiMesh *mesh = scene->mMeshes[0];

	if (pass == FINAL_PASS) {	
		GLint psize = glGetUniformLocation(shader->programID(), "pixelSize");
		glUniform2f(psize, 1.0 / normalsBuffer->getWidth(), 1.0 / normalsBuffer->getHeight());

		setMaterial(scene->mMaterials[mesh->mMaterialIndex], shader);
		setTextures(diffuse, specular, shader, normalsBuffer);
	} else if (pass == NORMALS_PASS) {
		GLint viewDepth = glGetUniformLocation(shader->programID(), "viewDepth");
		glUniform1f(viewDepth, Z_FAR - Z_NEAR);
	}

	setMeshData(mesh, shader, pass);

	glDrawElements(GL_TRIANGLES, 3 * mesh->mNumFaces, GL_UNSIGNED_INT, &indexBuffer[0]);

	glPopMatrix();
}
