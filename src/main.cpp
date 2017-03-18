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

using std::vector;
using glm::vec3;
using std::complex;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool keys[1024];
int windowWidth = 800, windowHeight = 600;
Camera cam = Camera(windowWidth, windowHeight);
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
	cam.lookAt(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.zoom(yoffset*0.2);
}
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

    // Positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

int main()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(windowWidth, windowHeight, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	Model model("data/model/tower/tower.obj");
	Model cube("data/model/shape/cube.obj");
	glm::vec3 lightPos(1.0f, 2.75f, -2.5f);
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

		lightPos.x = cos(lastFrame)*4.0f;
		lightPos.z = sin(lastFrame)*4.0f - 5.0f;
		cam.move(deltaTime, keys[GLFW_KEY_A], keys[GLFW_KEY_D], keys[GLFW_KEY_W], keys[GLFW_KEY_S]);
		glm::mat4 vp = cam.getViewProjection();
		glm::mat4 modelMat;

		//draw lamp cube
		lampShader.Use();
		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewProjLoc = glGetUniformLocation(lampShader.Program, "viewProj");
		glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(vp));
		//modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		modelMat = glm::translate(glm::mat4(1.0f), lightPos); // Translate it down a bit so it's at the center of the scene
		modelMat = glm::scale(modelMat,vec3(0.01f,0.01f,0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		cube.Draw(lampShader);
		glBindVertexArray(0);

		defaultShader.Use();
		modelLoc = glGetUniformLocation(defaultShader.Program, "model");
		viewProjLoc = glGetUniformLocation(defaultShader.Program, "viewProj");
		GLint viewPosLoc = glGetUniformLocation(defaultShader.Program, "viewPos");
		glUniform3f(viewPosLoc, cam.getPos()->x, cam.getPos()->y, cam.getPos()->z);
		glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(vp));

		GLint matShineLoc = glGetUniformLocation(defaultShader.Program, "material.shininess");
		glUniform1f(matShineLoc, 8.0f);

        // Directional light
        glUniform3f(glGetUniformLocation(defaultShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // Point light 1
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "pointLights[3].quadratic"), 0.032);

		//Spot light
		glUniform3f(glGetUniformLocation(defaultShader.Program, "spotlight.position"), cam.getPos()->x, cam.getPos()->y, cam.getPos()->z);
		glUniform3f(glGetUniformLocation(defaultShader.Program, "spotlight.direction"), cam.getFront()->x, cam.getFront()->y, cam.getFront()->z);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "spotlight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "spotlight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(defaultShader.Program, "spotlight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "spotlight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "spotlight.linear"), 0.02f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "spotlight.quadratic"), 0.02f);
        glUniform1f(glGetUniformLocation(defaultShader.Program, "spotlight.cutOff"), glm::cos(glm::radians(5.f)));
        glUniform1f(glGetUniformLocation(defaultShader.Program, "spotlight.outerCutOff"), glm::cos(glm::radians(25.f)));

		for(int i=0;i<5;i++){
		// Draw the loaded model
		modelMat = glm::scale(glm::mat4(1.0f),vec3(0.5f,0.5f,0.5f)*(i/10.0f));
		modelMat = glm::translate(modelMat, glm::vec3(sin(i)*4.0f, cos(i)*4.0f, -5.0f));
		//modelMat = glm::rotate(modelMat, (cos(lastFrame) + sin(i/4.0f))*4.0f, vec3(i/3,1.0f,i/5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

		model.Draw(defaultShader);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


