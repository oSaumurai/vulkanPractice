#include "model/Model.h"
#include <cassert>
#include <cstring>


Model::Model(EngineDevice& device, const std::vector<Vertex>& vertices)
    :m_device(device)
{
    createVertexBuffers(vertices);
}

Model::~Model()
{
    vkDestroyBuffer(m_device.device(), vertexBuffer, nullptr);
    vkFreeMemory(m_device.device(), vertexBufferMemory, nullptr);
}

void Model::createVertexBuffers(const std::vector<Vertex>& vertices)
{
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
    m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        vertexBuffer, vertexBufferMemory);

    void* data;
    vkMapMemory(m_device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_device.device(), vertexBufferMemory);
}
void Model::draw(VkCommandBuffer commandbuffer)
{
    vkCmdDraw(commandbuffer, vertexCount, 1, 0, 0);
}

void Model::bind(VkCommandBuffer commandbuffer)
{
    VkBuffer buffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandbuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescription()
{
    std::vector<VkVertexInputBindingDescription> bindingDescription(1);
    bindingDescription[0].binding = 0;
    bindingDescription[0].stride = sizeof(Vertex);
    bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
     
    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescription()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
    attributeDescription[0].binding = 0;
    attributeDescription[0].location = 0;
    attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription[0].offset = offsetof(Vertex, position);
    
    attributeDescription[1].binding = 0;
    attributeDescription[1].location = 1;
    attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription[1].offset = offsetof(Vertex,color);
    return attributeDescription;
}


