#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "Mesh.h"
using namespace std;

class Model
{
public:
	/*  Functions   */
	Model(string path)
	{
		this->loadModel(path);
	}
	Model(Mesh &mesh)
	{
		this->meshes.push_back(mesh);
	}
	void Draw(Shader shader);
private:
	/*  Model Data  */
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void calculateTangents(aiMesh* mesh, vector<Vertex> *vertices);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char * path, string directory);
};