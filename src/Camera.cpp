#include "Camera.h"

void Camera::move(float dt, bool left, bool right, bool up, bool down)
{
	GLfloat cameraSpeed = 5.0f * dt;
	if (up)
		_pos += cameraSpeed * _front;
	if (down)
		_pos -= cameraSpeed * _front;
	if (left)
		_pos -= glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
	if (right)
		_pos += glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
}

void Camera::lookAt(double xpos, double ypos)
{
	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	GLfloat xoffset = xpos - _lastX;
	GLfloat yoffset = _lastY - ypos; // Reversed since y-coordinates range from bottom to top
	_lastX = xpos;
	_lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	_yaw += xoffset;
	_pitch += yoffset;
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

}

void Camera::zoom(double yOffset)
{
	if (_fov >= 1.0f && _fov <= 45.0f)
		_fov -= yOffset;
	if (_fov <= 1.0f)
		_fov = 1.0f;
	if (_fov >= 45.0f)
		_fov = 45.0f;
}

glm::mat4 Camera::getViewProjection()
{
	getView();
	getProjection();
	return _projection*_view;
}

const glm::mat4* Camera::getView()
{
	_front.x = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
	_front.y = sin(glm::radians(_pitch));
	_front.z = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));
	_front = glm::normalize(_front);
	_view = glm::lookAt(_pos, _pos + _front, _up);
	return &_view;
}

const glm::mat4* Camera::getProjection()
{
	_projection = glm::perspective(_fov, _windowWidth / _windowHeight, 0.1f, 100.0f);
	return &_projection;
}

const glm::vec3* Camera::getPos() const
{
	return &_pos;
}

const glm::vec3* Camera::getFront() const
{
	return &_front;
}
