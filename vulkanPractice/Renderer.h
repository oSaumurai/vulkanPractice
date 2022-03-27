#pragma once
#include "Pipeline/Pipeline.h"
#include "SwapChain/SwapChain.h"
//std
#include <memory>
#include <vector>
class Renderer
{
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer &) = delete;
	Renderer& operator&=(const Renderer &) = delete;

	void run();
	//void update();
	//void draw();
private:
	void loadModel();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffer();
	void drawFrame();
	void recreateSwapChain();

	void recordCommandBuffer(uint32_t);
	void freeCommandBuffer();

	Window window{1280, 720, "Kirara"};
	EngineDevice m_device{ window };
	std::unique_ptr<SwapChain> swapChain;
	VkPipelineLayout pipelineLayout;

	std::unique_ptr<Pipeline> m_pipeLine;
	std::vector<VkCommandBuffer> commandBuffers;
	std::unique_ptr<Model> m_model; 
};

