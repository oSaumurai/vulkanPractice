#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <iostream>
#include <stdexcept>
class Window
{
public:
	Window(const int width, const int height, const std::string& title);
	Window(const Window &) = delete;
	Window& operator&=(const Window &) = delete;

	~Window();
	bool shouldClose();
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	void resetWindowResizeFlag() { frameBufferResizaed = false; }
	bool wasWindowResizeFlag() { return frameBufferResizaed; }

	VkExtent2D getExtent();
private:
	void initWindow();
	void initGLfw();

	static void frameBufferResizeCallback(GLFWwindow*, int width, int height);
private:
	bool frameBufferResizaed = false;
	int height;
	int width;

	GLFWwindow* _window;
	std::string title;
};

