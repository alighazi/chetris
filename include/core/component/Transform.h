#pragma once

#include "../Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

class Transform : public Component {
public:
	static const ComponentType TYPE = COMPONENT_TRANSFORM;

	Transform();
	inline Transform(const glm::vec3 &position){ transform_ = glm::translate(glm::mat4(1.0f), position); }
	inline Transform(float x, float y, float z){ transform_ = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));}

	~Transform() = default;

	inline glm::mat4 transform() const { return transform_; }
	inline void setTransform(const glm::mat4 &transform) { transform_ = transform; }
	inline void setRotation(const glm::vec3 &axis, float angle) { transform_ = glm::rotate(transform_, angle, axis);}

	inline ComponentType type() const{ return TYPE; }

private:
	glm::mat4 transform_;
};