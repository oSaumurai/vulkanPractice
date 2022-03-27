#include "Kirara_App.h"
#include "SimpleRenderSystem.h"

#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>

Kirara_App::Kirara_App()
{
	loadGameObjects();
}

Kirara_App::~Kirara_App()
{
}

void Kirara_App::run()
{
	SimpleRenderSystem simpleRenderSystem{ m_device, renderer.getSwapChainRenderPass() };
	while (!window.shouldClose()) {
		glfwPollEvents();

		if (auto commandBuffer = renderer.beginFrame()) {
			renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObject(commandBuffer, m_gameObjects);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
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
