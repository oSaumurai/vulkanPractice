#pragma once
#include "../EngineDevice.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


class Model
{
public:
	struct Vertex {
		glm::vec2 position;

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
	};

	Model(EngineDevice &device, const std::vector<Vertex>& vertices);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandbuffer);
	void draw(VkCommandBuffer commandbuffer);

private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);

	EngineDevice& m_device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};

