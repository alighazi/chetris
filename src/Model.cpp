#include "Model.h"
#include "Image.h"

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
	cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
	return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));

    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process all the node's meshes (if any)
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
	aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
	this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // Data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
	Vertex vertex;
	glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
	// Positions
	vector.x = mesh->mVertices[i].x;
	vector.y = mesh->mVertices[i].y;
	vector.z = mesh->mVertices[i].z;
	vertex.Position = vector;
	// Normals
	vector.x = mesh->mNormals[i].x;
	vector.y = mesh->mNormals[i].y;
	vector.z = mesh->mNormals[i].z;
	vertex.Normal = vector;

	if (mesh->HasTangentsAndBitangents())
	{
	    vector.x = mesh->mTangents[i].x;
	    vector.y = mesh->mTangents[i].y;
	    vector.z = mesh->mTangents[i].z;
	    vertex.Tangent = vector;

	    vector.x = mesh->mBitangents[i].x;
	    vector.y = mesh->mBitangents[i].y;
	    vector.z = mesh->mBitangents[i].z;
	    vertex.BitTangent = vector;
	}

	// Texture Coordinates
	if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
	{
	    glm::vec2 vec;
	    // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
	    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
	    vec.x = mesh->mTextureCoords[0][i].x;
	    vec.y = mesh->mTextureCoords[0][i].y;
	    vertex.TexCoords = vec;
	}
	else
	    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

	vertices.push_back(vertex);
    }

    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
	aiFace face = mesh->mFaces[i];
	// Retrieve all indices of the face and store them in the indices vector
	for (GLuint j = 0; j < face.mNumIndices; j++)
	    indices.push_back(face.mIndices[j]);
    }

    if (!mesh->HasTangentsAndBitangents())
    {
	calculateTangents(mesh, &vertices);
    }
    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	// Same applies to other texture as the following list summarizes:
	// Diffuse: texture_diffuseN
	// Specular: texture_specularN
	// Normal: texture_normalN

	// 1. Diffuse maps
	vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. Specular maps
	vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. Normal maps
	vector<Texture> normalMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    // Return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
	aiString str;
	mat->GetTexture(type, i, &str);
	GLboolean skip = false;
	for (GLuint j = 0; j < textures_loaded.size(); j++)
	{
	    if (textures_loaded[j].path == str)
	    {
		textures.push_back(textures_loaded[j]);
		skip = true;
		break;
	    }
	}
	if (!skip)
	{ // If texture hasn't been loaded already, load it
	    Texture texture;
	    texture.id = TextureFromFile(str.C_Str(), this->directory);
	    texture.type = typeName;
	    texture.path = str;
	    textures.push_back(texture);
	    this->textures_loaded.push_back(texture); // Add to loaded textures
	}
    }
    return textures;
}

GLint Model::TextureFromFile(const char *path, string directory)
{
    //Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    ImageInfo img = ImageInfo(filename.c_str());
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.pixels);
    std::cout << path << "-> w: " << img.width << "h: " << img.height << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    img.free();
    return textureID;
}

void Model::Draw(Shader shader)
{
    for (GLuint i = 0; i < this->meshes.size(); i++)
	this->meshes[i].Draw(shader);
}

void Model::calculateTangents(aiMesh *mesh, vector<Vertex> *vertices)
{
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
	aiFace face = mesh->mFaces[i];
	// Retrieve all indices of the face and store them in the indices vector
	for (GLuint j = 0; j < face.mNumIndices; j += 3)
	{

	    Vertex &v0 = (*vertices)[face.mIndices[j]];
	    Vertex &v1 = (*vertices)[face.mIndices[j + 1]];
	    Vertex &v2 = (*vertices)[face.mIndices[j + 2]];

	    glm::vec3 edge1 = v1.Position - v0.Position;
	    glm::vec3 edge2 = v2.Position - v0.Position;

	    float deltaU1 = v1.TexCoords.x - v0.TexCoords.x;
	    float deltaV1 = v1.TexCoords.y - v0.TexCoords.y;
	    float deltaU2 = v2.TexCoords.x - v0.TexCoords.x;
	    float deltaV2 = v2.TexCoords.y - v0.TexCoords.y;

	    float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

	    glm::vec3 tangent, bitangent;
	    tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
	    tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
	    tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

	    bitangent.x = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);
	    bitangent.y = f * (-deltaU2 * edge1.y - deltaU1 * edge2.y);
	    bitangent.z = f * (-deltaU2 * edge1.z - deltaU1 * edge2.z);

	    v0.Tangent += tangent;
	    v1.Tangent += tangent;
	    v2.Tangent += tangent;
	}
    }

    for (GLuint i = 0; i < vertices->size(); i++)
    {
	Vertex &v = (*vertices)[i];
	v.Tangent = glm::normalize(v.Tangent);
	v.BitTangent = glm::normalize(v.BitTangent);
    }
}
