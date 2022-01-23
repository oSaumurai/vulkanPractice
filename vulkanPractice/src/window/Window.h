#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <iostream>
#include <stdexcept>
const int HEIGHT = 1280, WIDTH = 720;
class Window
{
public:
	Window(const int width, const int height, const std::string& title);
	Window(const Window &) = delete;
	Window& operator&=(const Window &) = delete;

	~Window();
	//virtual void Update();
	//virtual void Draw();
	//void Finished();
	bool shouldClose();
	void print_vk_statics();
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	VkExtent2D getExtent();
private:
	void initWindow();
	void initGLfw();
private:
	uint32_t extensionCount = 0;
	GLFWwindow* _window;
	int window_height = 1280;
	int window_width = 720;
	std::string title;
};

