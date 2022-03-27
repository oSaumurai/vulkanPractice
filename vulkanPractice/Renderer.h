#pragma once
#include "SwapChain/SwapChain.h"
#include <glm/glm.hpp>
//std
#include <memory>
#include <vector>
#include <cassert>

class Renderer
{
public:
	Renderer(Window& window,EngineDevice& m_device);
	~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator&=(const Renderer&) = delete;

	VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
	bool isFrameInProgress() const { return isFrameStarted; }
	
	VkCommandBuffer getCurrentCommandBuffer() const {
		assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
		return commandBuffers[currentFrameIndex];
	}

	int getFrameIndex() const {
		assert(isFrameStarted && "Cannot get frame index when frame not in progress");
		return currentFrameIndex;
	}

	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
	void createCommandBuffer();
	void freeCommandBuffer();
	void recreateSwapChain();

	Window& window;
	EngineDevice& m_device;
	std::unique_ptr<SwapChain> swapChain;
	std::vector<VkCommandBuffer> commandBuffers;

	uint32_t currentImageIndex;
	int currentFrameIndex{ 0 };
	bool isFrameStarted{false};
};

