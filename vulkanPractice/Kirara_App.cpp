#include "Kirara_App.h"
#include "SimpleRenderSystem.h"

#include <glm/gtc/constants.hpp>
#include <iostream>
#include <array>
#include <chrono>

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
	Camera camera{};
    //camera.setViewDirection(glm::vec3(0.f), glm::vec3(.5f, .0f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 0.5f));

    auto currentTime = std::chrono::high_resolution_clock::now();

	while (!window.shouldClose()) {
		glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;
 
        float aspect = renderer.getAspectRatio();
	    camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);
		if (auto commandBuffer = renderer.beginFrame()) {
			renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObject(commandBuffer, m_gameObjects, camera);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}
	vkDeviceWaitIdle(m_device.device());
}

void Kirara_App::loadGameObjects()
{

    std::shared_ptr<Model> model = createCubeModelWIndex(m_device);
    auto cube = GameObject::createGameObj();
    cube.model = model;
    cube.transform.translation = { .0f,.0f,.5f };
    cube.transform.scale = { .2f, .2f, .2f };
    cube.color = { .1f,.8f,.1f };
    m_gameObjects.push_back(std::move(cube));

}
// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
std::unique_ptr<Model> Kirara_App::createCubeModelWIndex(EngineDevice& device) {
    Model::Builder modelBuilder{};
    modelBuilder.vertices = {
        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };
    modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

    return std::make_unique<Model>(device, modelBuilder);
}
