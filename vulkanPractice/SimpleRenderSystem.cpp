#include "SimpleRenderSystem.h"

#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>
struct SimplePushConstantData {
	glm::mat2 tranform{ 1.0f };
	glm::vec2 offset;
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

void SimpleRenderSystem::renderGameObject(VkCommandBuffer commandBuffer, std::vector<GameObject>& m_gameObjects)
{
	m_pipeLine->bind(commandBuffer);
	for (auto& obj : m_gameObjects) {
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
		SimplePushConstantData push{};
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
		push.tranform = obj.transform2d.mat2();

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
