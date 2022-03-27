#include "Kirara_App.h"
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>
struct SimplePushConstantData {
	glm::mat2 tranform{1.0f};
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

Kirara_App::Kirara_App()
{
	//window.print_vk_statics();
	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffer();
}

Kirara_App::~Kirara_App()
{
	vkDestroyPipelineLayout(m_device.device(), pipelineLayout, nullptr);
}

void Kirara_App::run()
{
	while (!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(m_device.device());
}

void Kirara_App::loadGameObjects()
{
	std::vector<Model::Vertex> vertices{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f} , {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};
	auto m_model = std::make_shared<Model>(m_device, vertices);
	auto triangle = GameObject::createGameObj();
	triangle.model = m_model;
	triangle.color = { .1f,.8f,.1f };
	triangle.transform2d.translation.x = 0.0f;
	triangle.transform2d.scale = { 1.0f, 1.0f };
	//triangle.transform2d.rotation = .25f * glm::two_pi<float>();
	m_gameObjects.push_back(std::move(triangle));

}

//private
void Kirara_App::createPipelineLayout()
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

void Kirara_App::createPipeline()
{
	assert(swapChain != nullptr && "Cannot create pipeline before swapchain");
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout");

	PipelineConfigInfo pipeConfigInfo{};
	Pipeline::defaultPipelineConfigInfo(pipeConfigInfo);
	pipeConfigInfo.renderPass = swapChain->getRenderPass();
	pipeConfigInfo.pipelineLayout = pipelineLayout;
	m_pipeLine = std::make_unique<Pipeline>(m_device, "src/shader/test.vert.spv", "src/shader/test.frag.spv", pipeConfigInfo);
}

void Kirara_App::freeCommandBuffer()
{
	vkFreeCommandBuffers(m_device.device(), m_device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

void Kirara_App::renderGameObject(VkCommandBuffer commandBuffer)
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

void Kirara_App::recordCommandBuffer(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to begin commandBuffers ");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width =  static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0,0}, swapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex],  0, 1, &scissor);

	renderGameObject(commandBuffers[imageIndex]);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to record commandBuffers");
	}
}


void Kirara_App::createCommandBuffer()
{
	commandBuffers.resize(swapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to allocate commandBuffers ");
	}
}

void Kirara_App::drawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain->acquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("App: failed to aquire swapchain image");
	}

	recordCommandBuffer(imageIndex);
	result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResizeFlag()) {
		window.resetWindowResizeFlag();
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS) {
		throw std::runtime_error("App: failed to present swapchain image");
	}
}

void Kirara_App::recreateSwapChain()
{
	auto extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window.getExtent();
		glfwWaitEvents();
	}

	if (swapChain == nullptr) {
		swapChain = std::make_unique<SwapChain>(m_device, extent);
	}
	else {
		swapChain = std::make_unique<SwapChain>(m_device, extent, std::move(swapChain));
		if (swapChain->imageCount() != commandBuffers.size()) {
			freeCommandBuffer();
			createCommandBuffer();
		}
	}
	vkDeviceWaitIdle(m_device.device());
	createPipeline();
}
