#include "core/system/InputSystem.h"
#include "core/system/RenderSystem.h"
#include "core/util/ModelManager.h"
#include "core/Game.h"

class OilDrum : public Game {
public:
	Entity* cube;
	OilDrum() {
		// We are going to use render, physics and input systems
		attach(new RenderSystem("Oil Drum"));
		//attach(new InputSystem());

		Shader* defaultShader=new Shader("shader/default.vert", "shader/default.frag");
		Shader* lampShader=new Shader("shader/default.vert", "shader/lamp.frag");

		Entity *drum = new Entity();
		drum->attach(new Transform(2.0, 1.0, -5.f));
		Model* m=ModelManager::instance().loadModel("data/model/oildrum/oildrum.obj");
		drum->attach(m);
		drum->attach(new Renderer(defaultShader));
		attach(drum);

		cube = new Entity();
		cube->attach(new Transform(2.0, 1.0, -5.f));
		cube->attach(ModelManager::instance().loadModel("data/model/shape/cube.obj"));
		cube->attach(new Renderer(lampShader));
		attach(cube);

	}

	void update(double deltaTime) {
		Transform* trans=cube->get<Transform>();
		trans->setTransform(glm::translate(trans->transform(), vec3(cos(deltaTime)*4.0f, 2.75f ,sin(deltaTime)*4.0f - 5.0f)));
	}
};


int main()
{
	OilDrum game;
	game.run();
	return 0;
}