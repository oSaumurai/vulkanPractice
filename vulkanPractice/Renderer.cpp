#include "Renderer.h"

#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>
struct SimplePushConstantData {
	glm::mat2 tranform{ 1.0f };
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

Renderer::Renderer(Window& window, EngineDevice& m_device)
	: window(window), m_device(m_device)
{
	recreateSwapChain();
	createCommandBuffer();
}

Renderer::~Renderer()
{
	freeCommandBuffer();
}

void Renderer::freeCommandBuffer()
{
	vkFreeCommandBuffers(m_device.device(), m_device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

void Renderer::createCommandBuffer()
{
	commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to allocate commandBuffers ");
	}
}


void Renderer::recreateSwapChain()
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
		std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
		swapChain = std::make_unique<SwapChain>(m_device, extent, oldSwapChain);
		
		if ( !oldSwapChain->compareSwapFormats(*swapChain.get()) ) {
			throw std::runtime_error("SwapChain image(or depth) format has been changed!");
		}
		//if (swapChain->imageCount() != commandBuffers.size()) {
		//	freeCommandBuffer();
		//	createCommandBuffer();
		//}
	}
}

VkCommandBuffer Renderer::beginFrame()
{
	assert(!isFrameStarted && "Can't call beginFrame while already in progress");
	auto result = swapChain->acquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return nullptr;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("App: failed to aquire swapchain image");
	}

	isFrameStarted = true;
	auto commandBuffer = getCurrentCommandBuffer();


	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to begin commandBuffers ");
	}

	return commandBuffer;
}

void Renderer::endFrame()
{
	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
	auto commandBuffer = getCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("App: failed to record commandBuffers");
	}

	auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResizeFlag()) {
		window.resetWindowResizeFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("App: failed to present swapchain image");
	}
	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Can't call beginSwapChain if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0,0}, swapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Can't call beginSwapChain if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}

