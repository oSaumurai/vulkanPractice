#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <vulkan/vulkan.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct TextureInfo {
	unsigned int id;
	std::string type;
	std::string path;
};
class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices,
		std::vector<uint32_t> indices,
		std::vector<TextureInfo> textures);
	~Mesh();

	//void Draw(Shader& shader);
	void Draw(VkCommandBuffer& commandBuffer);
	void Destroy();
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<TextureInfo> textures;

private:

	//unsigned int VBO, EBO;
private:
	void SetupMesh();
};

