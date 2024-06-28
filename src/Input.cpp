#include "Input.hpp"

bool Input::keyboardPress(int key)
{

	if (glfwGetKey(Input::window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

bool Input::mouseHold(int button)
{

	if (glfwGetMouseButton(Input::window, button) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

glm::vec2 Input::mouseDelta()
{

	return glm::vec2(0, 0);
}

glm::vec2 Input::mousePos()
{
	double x, y;
	glfwGetCursorPos(Input::window, &x, &y);
	return glm::vec2(x, y);
	
}