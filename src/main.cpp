#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <complex>
#include <cmath>

#include "shader.h"
#include "Image.h"
#include "Camera.h"
#include "Model.h"
#include "Light.hpp"
#include "GameWorld.h"
#include "GameModel.h"

using std::vector;
using glm::vec3;
using std::complex;

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

int main()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(GameWorld::WINDOW_WIDTH, GameWorld::WINDOW_HEIGHT, "Simple example", NULL, NULL);
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

	Shader defaultShader("shader/default.vert", "shader/default.frag");
	Shader lampShader("shader/default.vert", "shader/lamp.frag");
	// Load models
	Model model("data/model/oildrum/oildrum.obj");
	GameModel cube("data/model/shape/cube.obj", &lampShader);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GameWorld::instance().getCam()->move(deltaTime, keys[GLFW_KEY_A], keys[GLFW_KEY_D], keys[GLFW_KEY_W], keys[GLFW_KEY_S]);
		glm::mat4 modelMat;

		cube.update(deltaTime, lastFrame);
		cube.render();

		// defaultShader.Use();
		// GLint modelLoc = glGetUniformLocation(defaultShader.Program, "model");
		// GLint viewProjLoc = glGetUniformLocation(defaultShader.Program, "viewProj");
		// GLint viewPosLoc = glGetUniformLocation(defaultShader.Program, "viewPos");
		// glUniform3f(viewPosLoc, GameWorld::current().getCam()->getPos()->x, cam.getPos()->y, cam.getPos()->z);
		// glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(vp));

		// GLint matShineLoc = glGetUniformLocation(defaultShader.Program, "material.shininess");
		// glUniform1f(matShineLoc, 8.0f);

        // // Directional light
		// dir_light.addToShader(defaultShader.Program,"dirLight");
		// // Point light
		// point_light1.addToShader(defaultShader.Program,"pointLights[0]");
		// point_light2.addToShader(defaultShader.Program,"pointLights[1]");
		// point_light3.addToShader(defaultShader.Program,"pointLights[2]");
		// point_light4.addToShader(defaultShader.Program,"pointLights[3]");
		// //Spot light
		// spot_light.setPosition(*cam.getPos());
		// spot_light.setDirection(*cam.getFront());
		// spot_light.addToShader(defaultShader.Program,"spotlight");


		// for(int i=0;i<10;i++){
		// // Draw the loaded model
		// modelMat = glm::scale(glm::mat4(1.0f),vec3(1.0f));
		// modelMat = glm::translate(modelMat, glm::vec3(sin(i)*4.0f, cos(i)*4.0f, -5.0f));
		// //modelMat = glm::rotate(modelMat, (cos(lastFrame) + sin(i/4.0f))*4.0f, vec3(i/3,1.0f,i/5));
		// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

		// model.Draw(&defaultShader);
		//}



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}