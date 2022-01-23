#pragma once
#include <string>
#include <vector>
#include "../EngineDevice.h"
struct PipelineConfigInfo {
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};
class Pipeline
{ 

public:
	Pipeline(EngineDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
	~Pipeline();

	Pipeline(const Pipeline&) = delete;
	void operator=(const Pipeline&) = delete;

	void bind(VkCommandBuffer commandbuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo,uint32_t width, uint32_t height);
private:
	static std::vector<char> readFile(const std::string& filepath);

	void createGraphcisPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
	
	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	EngineDevice& m_device;
	VkPipeline graphicPipeLine;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

};

