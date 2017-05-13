#ifndef CORE_UTIL_MODELMANAGER_H
#define CORE_UTIL_MODELMANAGER_H
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../component/Model.h"
#include "Mesh.h"

class ModelManager
{
    public:
        Model* loadModel(string path);
        ModelManager() = default;
        ~ModelManager();
            //singletone
        static ModelManager& instance() 
        {
            static ModelManager instance;
            return instance;
        }
        ModelManager(ModelManager const&)        = delete;
        void operator = (ModelManager const&)    = delete;
        std::map<std::string, Texture*> textures_;
        
    private:
        std::string directory_;
        std::map<std::string, Model*> models_;  

        void processNode(aiNode* node, const aiScene* scene, Model *model);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
        int TextureFromFile(const char* filename);
        void calculateTangents(aiMesh* mesh, vector<Vertex> *vertices);
};

#endif