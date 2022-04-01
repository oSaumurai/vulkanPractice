#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <memory>
#include <iostream>

class AssimpLoader
{
public:
	std::vector<Mesh> meshes;
public:
	void LoadModel(const std::vector<Vertex>& vetices, const std::vector<uint32_t>& indices, const std::string& filepath);
	
private:

};

