#pragma once
#include "GameObject.h"
#include "Renderer.h"
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
private:
	void loadGameObjects();
	std::unique_ptr<Model> createCubeModel(EngineDevice& device);
	std::unique_ptr<Model> createCubeModelWIndex(EngineDevice& device);
	Window window{1280, 720, "Kirara"};
	EngineDevice m_device{ window };
	Renderer renderer{ window, m_device };

	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<GameObject> m_gameObjects; 
};

