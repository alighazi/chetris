#ifndef CORE_COMPONENT_RENDERER_H
#define CORE_COMPONENT_RENDERER_H

#include "../Component.h"
#include "../util/Shader.h"

#include <string>

class Renderer : public Component {
public:
	static const ComponentType TYPE = COMPONENT_RENDERER;

	Renderer(Shader* shader);

	~Renderer() = default;

	Shader* shader() const;

	void setVisible(bool visible);
	void setBlend(bool enabled);
	void setOverlayed(bool enabled);
	void setDepthTest(bool enabled);

	bool visible() const;
	bool blend() const;
	bool overlayed() const;
	bool depthTest() const;

	ComponentType type() const;

private:
	Shader* shader_;

	bool visible_;
	bool blend_;
	bool overlayed_;
	bool depthTest_;
};
#endif