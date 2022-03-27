#pragma once
#include <model/model.h>

#include <memory>

struct Transform2DComponent {
	glm::vec2 translation{};
	glm::vec2 scale{ 1.0f, 1.0f };
	float rotation = 1.f;

	glm::mat2 mat2() 
	{	
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotMat{ { c,s }, { -s,c } };
		glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
		return rotMat * scaleMat;
	}

};
class GameObject
{
public:
	using id_t = unsigned int;

	static GameObject createGameObj() {
		static id_t currentId = 0;
		return GameObject(currentId++);
	}

	id_t getId() { return id; }

	std::shared_ptr<Model> model{};
	glm::vec3 color{};
	Transform2DComponent transform2d;
private:
	GameObject(id_t objID): id(objID) {}
	id_t id; 
};

