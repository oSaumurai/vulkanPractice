#pragma once
#include "../EngineDevice.h"
//#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Model
{
public:
	struct Vertex 
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;


		static std::vector<VkVertexInputBindingDescription>   getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
	};

	struct Mesh 
	{
		Mesh(std::vector<Vertex>& vertices,
			std::vector<uint32_t>& indices) : vertices(vertices), indices(indices) {};
		~Mesh() {};

	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		//std::vector<TextureInfo> textures;
	public:
		void Draw(VkCommandBuffer&);
	};

	struct Builder 
	{
		//std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		std::vector<Mesh> meshes{};

		void loadModel(const std::string& filename);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

	Model(EngineDevice& device, const Model::Builder& builder);
	Model(EngineDevice& device, const Model::Mesh& mesh);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandbuffer);
	void draw(VkCommandBuffer commandbuffer);

	static std::vector<std::shared_ptr<Model>> createModelFromFile(EngineDevice&, const std::string& filepath);

private:
	void createMesh(const std::vector<Mesh>& meshes);
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);

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

