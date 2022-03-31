#pragma once
#include "../EngineDevice.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Model
{
public:
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;


		static std::vector<VkVertexInputBindingDescription>   getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
	};

	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		void loadModel(const std::string& filename);
	};

	Model(EngineDevice &device, const Model::Builder& builder);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandbuffer);
	void draw(VkCommandBuffer commandbuffer);

private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);
	std::unique_ptr<Model> createModelFromFile(EngineDevice&, const std::string& filepath);

private:
	EngineDevice& m_device;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t indexCount;
};

