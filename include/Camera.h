#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::mat4 _view, _projection;
	GLfloat _fov = 45.f;
	GLfloat _yaw = -90.f, _pitch = 0.f;
	glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 _front= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat _lastX = 0, _lastY = 0;
	bool _firstMouse = true;
	GLfloat _windowWidth, _windowHeight;
public:
	Camera(int windowWidth, int windowHeight) :_windowWidth(windowWidth), _windowHeight(windowHeight) {}
	void move(float dt, bool left, bool right, bool up, bool down);
	void lookAt(double xpos, double ypos);
	void zoom(double yOffset);
	glm::mat4 getViewProjection();
	const glm::vec3* getPos() const;
	const glm::vec3* getFront() const;
};