#ifndef CORE_UTIL_MESH_H
#define CORE_UTIL_MESH_H

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BitTangent;
};

struct Texture {
	int id;
	int uniform;
	string type;
	std::string path;
};
class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture*> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture*> textures);
	void Draw(Shader* shader);
private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};
#endif