#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <complex>
#include <cmath>


#include "core/system/RenderSystem.h"
#define GLDBG printf("%s:%i - %x\n", __FILE__, __LINE__, glGetError());
#define CAPTURE_MOUSE

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool keys[1024];
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	GameWorld::instance().getCam()->lookAt(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	GameWorld::instance().getCam()->zoom(yoffset*0.2);
}
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

DirectionalLight dir_light(vec3(-0.2f, -1.0f, -0.3f),vec3(0.05f, 0.05f, 0.05f),vec3(0.4f, 0.4f, 0.4f)*0.1f,vec3(0.5f, 0.5f, 0.5f));
SpotLight spot_light(vec3(0.05f, 0.05f, 0.05f),vec3(0.4f, 0.4f, 0.4f),vec3(0.5f, 0.5f, 0.5f),vec3(0.0f), vec3(0.0f),1.0f, 0.02f, 0.02f,glm::cos(glm::radians(20.f)),glm::cos(glm::radians(25.f)));


PointLight point_light1(vec3(0.05f, 0.05f, 0.05f),vec3( 0.8f, 0.0f, 0.4f),vec3(1.0f, 1.0f, 1.0f), vec3( 0.7f,  0.2f,  2.0f), 1.0f,0.09f,0.032f);
PointLight point_light2(vec3(0.05f, 0.05f, 0.05f),vec3( 0.4f, 0.2f, 0.8f),vec3(1.0f, 1.0f, 1.0f), vec3( 2.3f, -3.3f, -4.0f), 1.0f,0.09f,0.032f);
PointLight point_light3(vec3(0.05f, 0.05f, 0.05f),vec3( 0.2f, 0.4f, 0.8f),vec3(1.0f, 1.0f, 1.0f), vec3(-4.0f,  2.0f, -12.0f), 1.0f,0.09f,0.032f);
PointLight point_light4(vec3(0.05f, 0.05f, 0.05f),vec3( 0.4f, 0.8f, 0.2f),vec3(1.0f, 1.0f, 1.0f), vec3( 0.0f,  0.0f, -3.0f), 1.0f,0.09f,0.032f);

RenderSystem::RenderSystem(std::string windowTitle, int windowWidth, int windowHeight){
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str() , NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	#ifdef CAPTURE_MOUSE
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	#endif
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
}

RenderSystem::~RenderSystem() {

}

void RenderSystem::update(double deltaTime) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GameWorld::instance().getCam()->move(deltaTime, keys[GLFW_KEY_A], keys[GLFW_KEY_D], keys[GLFW_KEY_W], keys[GLFW_KEY_S]);

		Shader* lastShader=nullptr;
		for (auto cache : meshCaches_) {
			const Entity *entity = cache.first;
			const Renderer *renderer = entity->get<Renderer>();
			Model *model = entity->get<Model>();
			const Transform* transform = entity->get<Transform>();
			Shader* sh = renderer->shader();

			if(lastShader!=sh){
				sh->Use();
				GLint matShineLoc = glGetUniformLocation(sh->Program, "material.shininess");
				glUniform1f(matShineLoc, 8.0f);
				// Directional light
				dir_light.addToShader(sh->Program,"dirLight");
				// Point light
				point_light1.addToShader(sh->Program,"pointLights[0]");
				point_light2.addToShader(sh->Program,"pointLights[1]");
				point_light3.addToShader(sh->Program,"pointLights[2]");
				point_light4.addToShader(sh->Program,"pointLights[3]");
				//Spot light
				spot_light.setPosition(*GameWorld::instance().getCam()->getPos());
				spot_light.setDirection(*GameWorld::instance().getCam()->getFront());
				spot_light.addToShader(sh->Program,"spotlight");	
				glm::mat4 vp = GameWorld::instance().getCam()->getViewProjection();
				glUniformMatrix4fv(glGetUniformLocation(sh->Program, "viewProj"), 1, GL_FALSE, glm::value_ptr(vp));
				glUniformMatrix4fv(glGetUniformLocation(sh->Program, "model"), 1, GL_FALSE, glm::value_ptr(transform->transform()));
				lastShader=sh;				
			}
			model->Draw(sh);
		}

		// Swap the screen buffers
		glfwSwapBuffers(window);
}

SystemType RenderSystem::type() const {
	return TYPE;
}

void RenderSystem::setBackgroundColor(float red, float green, float blue) {
	glClearColor(red, green, blue, 1.0);
}

void RenderSystem::onChangeType(Entity *entity, int fromType, int toType) {
	// const bool fromCamera = (fromType & Camera::TYPE) && (fromType & Transform::TYPE);
	// const bool toCamera = (toType & Camera::TYPE) && (toType & Transform::TYPE);

	// if (!fromCamera && toCamera) {
	// 	// A camera was attached. Use it if a camera of its type is not already set.
	// 	if (!cameras_[entity->get<Camera>()->cameraType()])
	// 		cameras_[entity->get<Camera>()->cameraType()] = entity;
	// } else if (fromCamera && !toCamera) {
	// 	// A camera was detached. If it is the currently set camera of its type, reset it.
	// 	if (cameras_[entity->get<Camera>()->cameraType()] == entity)
	// 		cameras_.erase(entity->get<Camera>()->cameraType());
	// }

	const bool fromModel = (fromType & Renderer::TYPE) && (fromType & Transform::TYPE) && (fromType & Model::TYPE);
	const bool toModel = (toType & Renderer::TYPE) && (toType & Transform::TYPE) && (toType & Model::TYPE);

	if (!fromModel && toModel) {
		// A mesh shape was attached
		addMeshCache(entity);
		entity->subscribe(MESSAGE_MESH_CHANGED, this);
	} else if (fromModel && !toModel) {
		// A mesh shape was detached
		entity->unsubscribe(MESSAGE_MESH_CHANGED, this);
		removeMeshCache(entity);
	}

	// const bool fromLight = (fromType & Light::TYPE) && (fromType & Transform::TYPE);
	// const bool toLight = (toType & Light::TYPE) && (toType & Transform::TYPE);

	// if (!fromLight && toLight) {
	// 	lights_.insert(entity);
	// } else if (fromLight && !toLight) {
	// 	lights_.erase(entity);
	// }
}

void RenderSystem::addMeshCache(Entity *entity) {
	MeshCache *cache = new MeshCache();
	cache->entity = entity;
	//cache->cameraType = entity->get<Renderer>()->cameraType();
	cache->shader = entity->get<Renderer>()->shader();
	cache->blend = entity->get<Renderer>()->blend();
	cache->depthTest = entity->get<Renderer>()->depthTest();

	glGenVertexArrays(1, &cache->vertexArrayObject.vertexArray);
	glGenBuffers(1, &cache->vertexArrayObject.buffer);
	meshCaches_[entity] = cache;

	//orderedMeshCaches_[cache->cameraType].insert(cache);

	//updateMeshCache(entity);
}

void RenderSystem::removeMeshCache(Entity *entity) {
	MeshCache *cache = meshCaches_[entity];

	meshCaches_.erase(entity);
	//orderedMeshCaches_[cache->cameraType].erase(cache);

	glDeleteBuffers(1, &cache->vertexArrayObject.buffer);

	delete cache;
}

void RenderSystem::onMessage(MessageType messageType, Entity *entity) {
	// updateMeshCache(entity);
}

void RenderSystem::onAttach(Game *game) {

}