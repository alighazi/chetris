#include "core/util/Mesh.h"
#include "core/util/ModelManager.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture*> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(GLvoid*)offsetof(Vertex, Tangent));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(GLvoid*)offsetof(Vertex, BitTangent));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr=1;
	int uniform=0;
	for (const auto& kv : ModelManager::instance().textures_)
	{
		stringstream ss;
		string number;
		string name = kv.second->type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; 
		else if (name == "texture_specular")
			ss << specularNr++;
		else if(name=="texture_normal") 
			ss << normalNr++;
		number = ss.str();
		glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number).c_str()), uniform);
		glActiveTexture(GL_TEXTURE0 +uniform ); // Activate proper texture unit before binding		
		glBindTexture(GL_TEXTURE_2D, kv.second->id);
		uniform++;
	}
	//glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}