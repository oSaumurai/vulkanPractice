#pragma once
#include "Pipeline/Pipeline.h"
#include "SwapChain/SwapChain.h"
//std
#include <memory>
#include <vector>
class Kirara_App
{
public:
	Kirara_App();
	~Kirara_App();
	Kirara_App(const Kirara_App &) = delete;
	Kirara_App& operator&=(const Kirara_App &) = delete;

	void run();
	//void update();
	//void draw();
private:
	void loadModel();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffer();
	void drawFrame();

	Window window{WIDTH, HEIGHT, "Kirara"};
	EngineDevice m_device{ window };
	SwapChain swapChain{ m_device, window.getExtent() };
	VkPipelineLayout pipelineLayout;

	std::unique_ptr<Pipeline> m_pipeLine;
	std::vector<VkCommandBuffer> commandBuffers;
	std::unique_ptr<Model> m_model;
};

