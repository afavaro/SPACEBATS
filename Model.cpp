
#include "Model.h"
#include "Camera.h"

#include <sys/stat.h>

using namespace std;
using namespace sf;

Image Model::white(1, 1, sf::Color(255, 255, 255));

Shader *Model::toonShader, *Model::normalShader;
Framebuffer *Model::normalsBuffer;

Model::Model() {
	scene = NULL;
	diffuse = specular = NULL;
	indexBuffer = NULL;
	collisionShape = NULL;
	transformation = btTransform::getIdentity();
	scaleFactor = 1.0;
}

Model::~Model() {
	delete diffuse;
	delete specular;
	delete collisionShape;
	delete[] indexBuffer;
}

void Model::setScaleFactor(float f){
	scaleFactor = f;
	collisionShape->setLocalScaling(btVector3(f, f, f));
}

void Model::setDiffuseImage(sf::Image* img){
	diffuse = img;
}

sf::Image* Model::getDiffuseImage(){
	return diffuse;
}

void Model::loadShaders() {
	normalShader = new Shader("shaders/normal");
	toonShader = new Shader("shaders/toon");
}

void Model::setNormalsBuffer(Framebuffer *fb) {
	normalsBuffer = fb;
}

btCollisionShape *Model::getCollisionShape() {
	return collisionShape;
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

	collisionShape = new btConvexHullShape(
			(btScalar *)mesh->mVertices, mesh->mNumVertices, sizeof(aiVector3D));
	btVector3 inertia(0, 0, 0);
	collisionShape->calculateLocalInertia(4.0, inertia);

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

void Model::setTransformation(btTransform &t) {
	transformation = t;
}

static void setMaterial(aiMaterial *mat, Shader *shader)
{
	aiColor3D color;

	GLint diffuse = glGetUniformLocation(shader->programID(), "Kd");
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	glUniform3f(diffuse, color.r, color.g, color.b);

	GLint ambient = glGetUniformLocation(shader->programID(), "Ka");
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	glUniform3f(ambient, color.r, color.g, color.b);
}

static void setTextures(Image *diffuse, Image *specular, Shader *shader, Framebuffer *normalsBuffer)
{
	GLint diff = glGetUniformLocation(shader->programID(), "diffuseMap");
	glUniform1i(diff, 1);
	glActiveTexture(GL_TEXTURE1);
	if (diffuse) diffuse->Bind();
	else Model::white.Bind();

	GLint norm = glGetUniformLocation(shader->programID(), "normalMap");
	glUniform1i(norm, 2);
	glActiveTexture(GL_TEXTURE2);
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

void Model::render(RenderPass pass) {
	Shader *shader;
	if (pass == NORMALS_PASS) {
		shader = normalShader;
		normalsBuffer->bind();
	} else
		shader = toonShader;

	glUseProgram(shader->programID());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	
	
	GLfloat glmat[16];
	transformation.getOpenGLMatrix(glmat);
	glMultMatrixf(glmat);
	glScalef(scaleFactor, scaleFactor, scaleFactor);

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

	if (pass == NORMALS_PASS) normalsBuffer->unbind();
}
