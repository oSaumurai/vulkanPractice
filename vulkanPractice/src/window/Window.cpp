#include "Window.h"

Window::Window(const int witdh, const int height, const std::string& _title)
	: window_height(height), window_width(witdh), title(_title)
{
	initGLfw();
	initWindow();
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::initWindow()
{
	initGLfw();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(window_height, window_width, title.c_str(), NULL, NULL);

	extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
}

void Window::initGLfw()
{
	if (!glfwInit()) {
		__debugbreak();
		std::cout << "error initlize glfw" << std::endl;
		glfwTerminate();
	}
}

void Window::print_vk_statics()
{
	std::cout << extensionCount;
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("faile to create window surface");
	}
}

VkExtent2D Window::getExtent()
{
	return VkExtent2D({ static_cast<uint32_t>(window_width), static_cast<uint32_t>(window_height) });
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(_window);
}
