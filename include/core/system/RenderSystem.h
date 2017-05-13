#ifndef CORE_SYSTEM_RENDERSYSTEM_H
#define CORE_SYSTEM_RENDERSYSTEM_H

#include <map>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../component/Renderer.h"
#include "../component/Transform.h"
#include "../component/Model.h"
#include "../Entity.h"
#include "../System.h"

#include "../util/Shader.h"
#include "Camera.h"
#include "Light.hpp"
#include "GameWorld.h"
#include "GameModel.h"

class RenderSystem : public System {
public:
	static const SystemType TYPE = SYSTEM_RENDER;

	RenderSystem(std::string windowTitle, int windowWidth = 1024, int windowHeight = 768);
	~RenderSystem();

	void update(double deltaTime);

	SystemType type() const;

	void setBackgroundColor(float red, float green, float blue);

private:
	GLFWwindow* window;
	struct VertexArrayObject {
		GLuint vertexArray;
		GLuint buffer;
		GLsizei vertexCount;
	};

	struct MeshCache {
		Entity *entity;

		VertexArrayObject vertexArrayObject;
		Shader* shader;
		bool blend;
		bool depthTest;
	};


	//std::map<CameraType, Entity *> cameras_;

	// All attached entities with their caches
	std::map<Entity *, MeshCache *> meshCaches_;

	// Mesh caches in the correct render order
	//std::map<CameraType, std::set<MeshCache *, DrawOrderCompare>> orderedMeshCaches_;

	//std::set<Entity *> lights_;

	// GLuint frameBuffer_;
	// GLuint depthBuffer_;
	// GLuint diffuseTexture_;
	// GLuint positionTexture_;
	// GLuint normalTexture_;
	// GLuint lightTexture_;

	// GLuint squareVertexArray_;
	// GLuint squareBuffer_;

	// VertexArrayObject lightVolume_;

	void addMeshCache(Entity *entity);
	void removeMeshCache(Entity *entity);
	// void updateMeshCache(Entity *entity);

	//void loadVertexArrayObject(MeshShape *mesh, VertexArrayObject *vertexArrayObject);

	void onChangeType(Entity *entity, int fromType, int toType);

	void onMessage(MessageType messageType, Entity *entity);

	void onAttach(Game *game);

	// GLuint generateTexture(GLint internalFormat, GLenum type);
};

#endif