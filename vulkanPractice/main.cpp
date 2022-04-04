#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <iostream>
#include <stdexcept>
#include "Kirara_App.h"

int main() {
    
    Kirara_App* app = new Kirara_App();
    app->run();
    return 0;
}