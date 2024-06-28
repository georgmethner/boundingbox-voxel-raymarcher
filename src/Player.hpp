#include <glm/glm.hpp>
#include "Input.hpp"

class Player
{
public:
	Input input;
	void Update(float deltaTime);
	glm::vec3 position = glm::vec3(20.0, 5.0, 40.0);
	glm::vec3 dir = glm::vec3(1.0, 0.0, 0.0);
	float speed = 8.1;
	float pitch = 0.0;
	float yaw = 0.0f;
};