#ifndef CORE_COMPONENT_MODEL_H
#define CORE_COMPONENT_MODEL_H
#include <string>
#include <assimp/Importer.hpp>

#include "../Component.h"
#include "../util/Mesh.h"

class Model : public Component{
public:
	static const ComponentType TYPE = COMPONENT_MODEL;

	ComponentType type() const {return TYPE;}

	Model() = default;
	~Model() = default;

	void notifyChanged() const;
	vector<Mesh> meshes_;  
    void Draw(Shader* shader)
    {
        for (int i = 0; i < this->meshes_.size(); i++)
        this->meshes_[i].Draw(shader);
    }  
private:
};

#endif