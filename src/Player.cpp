#include "Player.hpp"

glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
glm::vec2 lastMousePos = glm::vec2(0, 0);
bool setLastMousePos;

void Player::Update(float deltaTime)
{
	if (input.mouseHold(1))
	{
		if (setLastMousePos == false)
		{
			lastMousePos = input.mousePos();
			setLastMousePos = true;
		}
		glfwSetInputMode(input.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		float xoffset = input.mousePos().x - lastMousePos.x;
		float yoffset = lastMousePos.y - input.mousePos().y;
		lastMousePos = input.mousePos();

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = glm::clamp(pitch, -90.0f, 90.0f);

		glm::vec3 front;
		front.x = cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw));
		dir = front;
	}
	else
	{
		setLastMousePos = false;
		glfwSetInputMode(input.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (input.keyboardPress(GLFW_KEY_W))
	{
		position += deltaTime * speed * dir;
	}
	if (input.keyboardPress(GLFW_KEY_A))
	{
		position += deltaTime * speed * -glm::normalize(glm::cross(dir, up));
	}
	if (input.keyboardPress(GLFW_KEY_S))
	{
		position += deltaTime * speed * -dir;
	}
	if (input.keyboardPress(GLFW_KEY_D))
	{
		position += deltaTime * speed * glm::normalize(glm::cross(dir, up));
	}
	if (input.keyboardPress(GLFW_KEY_SPACE))
	{
		position += deltaTime * speed * up;
	}
	if (input.keyboardPress(GLFW_KEY_LEFT_SHIFT))
	{
		position += deltaTime * speed * -up;
	}


}




