#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input
{
public:
	GLFWwindow* window;
	bool keyboardPress(int key);
	bool mouseHold(int key);
	glm::vec2 mouseDelta();
	glm::vec2 mousePos();
};