#include "SimpleRenderSystem.h"

#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>
struct SimplePushConstantData {
	glm::mat4 tranform{ 1.0f };
	alignas(16) glm::vec3 color;
};

SimpleRenderSystem::SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass)
	:m_device(device)
{
	createPipelineLayout();
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_device.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	std::cout << "kirara app: pipelineLayout" << std::endl;
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("App: failed to create pipelineLayout");
	}
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout");

	PipelineConfigInfo pipeConfigInfo{};
	Pipeline::defaultPipelineConfigInfo(pipeConfigInfo);
	pipeConfigInfo.renderPass = renderPass;
	pipeConfigInfo.pipelineLayout = pipelineLayout;
	m_pipeLine = std::make_unique<Pipeline>(m_device, "src/shader/test.vert.spv", "src/shader/test.frag.spv", pipeConfigInfo);
}

void SimpleRenderSystem::renderGameObject(VkCommandBuffer commandBuffer, std::vector<GameObject>& m_gameObjects, const Camera& camera)
{
	m_pipeLine->bind(commandBuffer);
	
	auto projectionView = camera.getProjection() * camera.getViewMatrix();

	for (auto& obj : m_gameObjects) {
		SimplePushConstantData push{};
		push.tranform = projectionView * obj.transform.mat4();
		push.color = obj.color;

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
