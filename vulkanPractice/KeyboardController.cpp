#include "KeyboardController.h"

void KeyboardController::moveInPlaneXZ(GLFWwindow* window, float dt, GameObject& object)
{
	glm::vec3 rotate{ 0 };
	if (glfwGetKey(window, keys.lookRight)== GLFW_PRESS) rotate.y += 1.f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
	if (glfwGetKey(window, keys.lookUp)   == GLFW_PRESS) rotate.x += 1.f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;
	if(glm::dot(rotate,rotate) > std::numeric_limits<float>::epsilon())
		object.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

	object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
	object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

	float yaw = object.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
	const glm::vec3 upDir{ 0.f, -1.f,0.f };

}
