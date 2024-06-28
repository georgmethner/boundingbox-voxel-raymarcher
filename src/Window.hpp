#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	void StartGLFW(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	GLFWwindow *getWindow() {
		return window;
	}

private:
	GLFWwindow *window;
};

