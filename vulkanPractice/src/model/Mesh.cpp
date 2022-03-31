#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
	:vertices(vertices), indices(indices)
{
	SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(VkCommandBuffer& commandBuffer)
{
}

void Mesh::Destroy()
{
}

void Mesh::SetupMesh()
{
	
}
