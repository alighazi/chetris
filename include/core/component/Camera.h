// #ifndef CORE_COMPONENT_CAMERA_H
// #define CORE_COMPONENT_CAMERA_H

// #include "../Component.h"

// #include <glm/glm.hpp>

// enum Direction {
//     LEFT        = 1 << 0,
//     RIGHT       = 1 << 1,
//     UP          = 1 << 2,
//     DOWN        = 1 << 3,
// };
// class Camera : public Component {
// public:
// 	static const ComponentType TYPE = COMPONENT_CAMERA;

// 	Camera(float fieldOfViewY, float zNear, float zFar) :fov_(fieldOfViewY), zNear_(zNear), zFar_(zFar){}
// 	~Camera() = default;


// 	CameraType cameraType() const;
// 	glm::mat4 projection(int width, int height) const;
// 	ComponentType type() const;

//     void move(float dt, bool left, bool right, bool up, bool down);
// 	void lookAt(double xpos, double ypos);
// 	void zoom(double yOffset);
// 	glm::mat4 viewProjection();
// 	const glm::vec3* pos() const;
// 	const glm::vec3* front() const;
// 	const glm::mat4* view();

// private:
// 	glm::mat4 view_, projection_;
// 	float fov_, zNear_,zFar_;
// 	float yaw_ = -90.f, pitch_ = 0.f;
// 	glm::vec3 pos_ = glm::vec3(0.0f, 0.0f, 1.0f);
// 	glm::vec3 front_= glm::vec3(0.0f, 0.0f, -1.0f);
// 	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
// 	GLfloat lastX_ = 0, lastY_ = 0;
// 	bool firstMouse_ = true;
// };

// #endif