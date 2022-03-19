#include "Window.h"

Window::Window(const int w, const int h, const std::string& _title)
	: title(_title), width(w), height(h)
{
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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, frameBufferResizeCallback);
}

void Window::initGLfw()
{
	if (!glfwInit()) {
		__debugbreak();
		std::cout << "error initlize glfw" << std::endl;
		glfwTerminate();
	}
}

void Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto m_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	m_window->frameBufferResizaed = true;
	m_window->width = width;
	m_window->height = height;
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("faile to create window surface");
	}
}

VkExtent2D Window::getExtent()
{
	return VkExtent2D({ static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(_window);
}
