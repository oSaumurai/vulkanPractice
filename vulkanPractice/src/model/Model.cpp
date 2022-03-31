#include "model/Model.h"
#include <cassert>
#include <cstring>


Model::Model(EngineDevice& device, const Model::Builder& builder)
    :m_device(device)
{
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Model::~Model()
{
    vkDestroyBuffer(m_device.device(), vertexBuffer, nullptr);
    vkFreeMemory(m_device.device(), vertexBufferMemory, nullptr);

    if (hasIndexBuffer) {
        vkDestroyBuffer(m_device.device(), indexBuffer, nullptr);
        vkFreeMemory(m_device.device(), indexBufferMemory, nullptr);
    }
}

void Model::createVertexBuffers(const std::vector<Vertex>& vertices)
{
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_device.device(), stagingBufferMemory);

    m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer, vertexBufferMemory);
    m_device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
    vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffers(const std::vector<uint32_t>& indices)
{
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 3;
    if (!hasIndexBuffer)
        return;
    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_device.device(), stagingBufferMemory);

    m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        indexBuffer, indexBufferMemory);
    m_device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
    vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

std::unique_ptr<Model> Model::createModelFromFile(EngineDevice&, const std::string& filepath)
{
    Builder builder{};
    builder.loadModel(filepath);
    std::cout << "Vertex Count: " << builder.vertices.size() << std::endl;
    return std::make_unique<Model>(m_device, builder);
}

void Model::draw(VkCommandBuffer commandbuffer)
{
    if (hasIndexBuffer) 
    {
        vkCmdDrawIndexed(commandbuffer, indexCount, 1, 0, 0, 0);
    }
    else 
    {
        vkCmdDraw(commandbuffer, vertexCount, 1, 0, 0);
    }
}

void Model::bind(VkCommandBuffer commandbuffer)
{
    VkBuffer buffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandbuffer, 0, 1, buffers, offsets);

    if (hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
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
    attributeDescription[0].offset = offsetof(Vertex, Position);
    
    attributeDescription[1].binding = 0;
    attributeDescription[1].location = 1;
    attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription[1].offset = offsetof(Vertex, Color);
    return attributeDescription;
}

void Model::Builder::loadModel(const std::string& filename)
{

}
