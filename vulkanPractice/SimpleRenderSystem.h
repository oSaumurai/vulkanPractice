#pragma once
#include "Pipeline/Pipeline.h"
#include "GameObject.h"
//std
#include <memory>
#include <vector>
class SimpleRenderSystem
{
public:
	SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass);
	~SimpleRenderSystem();

	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator&=(const SimpleRenderSystem&) = delete;

	void renderGameObject(VkCommandBuffer, std::vector<GameObject>&);
private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);

	EngineDevice& m_device;

	VkPipelineLayout pipelineLayout;
	std::unique_ptr<Pipeline> m_pipeLine;
};

