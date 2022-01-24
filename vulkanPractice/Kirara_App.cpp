#include "Kirara_App.h"
#include <iostream>
#include <array>
Kirara_App::Kirara_App()
{
	//window.print_vk_statics();
	loadModel();
	createPipelineLayout();
	createPipeline();
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

}

void Kirara_App::loadModel()
{
	std::vector<Model::Vertex> vertices{
		{{0.0f, -0.5f}},
		{{0.5f, 0.5f}},
		{{-0.5f, 0.5f}}
	};
	m_model = std::make_unique<Model>(m_device, vertices);
}

//private
void Kirara_App::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	std::cout << "kirara app: pipelineLayout" << std::endl;
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("App: failed to create pipelineLayout");
	}
}

void Kirara_App::createPipeline()
{
	PipelineConfigInfo pipeConfigInfo{};
	Pipeline::defaultPipelineConfigInfo(pipeConfigInfo, WIDTH, HEIGHT);
	pipeConfigInfo.renderPass = swapChain.getRenderPass();
	pipeConfigInfo.pipelineLayout = pipelineLayout;
	m_pipeLine = std::make_unique<Pipeline>(m_device, "src/shader/test.vert.spv", "src/shader/test.frag.spv", pipeConfigInfo);
	

}

void Kirara_App::createCommandBuffer()
{
	commandBuffers.resize(swapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to allocate commandBuffers ");
	}

	for (int i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("App: failed to begin commandBuffers ");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain.getRenderPass();
		renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		m_pipeLine->bind(commandBuffers[i]);
		m_model   ->bind(commandBuffers[i]);
		m_model   ->draw(commandBuffers[i]);

		//move to model
		//vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("App: failed to record commandBuffers");
		}
	}
}

void Kirara_App::drawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain.acquireNextImage(&imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("App: failed to aquire swapchain image");
	}

	result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("App: failed to present swapchain image");
	}
}
