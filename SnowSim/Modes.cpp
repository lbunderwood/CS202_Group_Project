// Created by Luke Underwood
// 4/25/2020
// SnowSim
// Modes.cpp
// Contains function definitions to enable multiple field "modes" so that
// the user can dynamically switch between a number of different fields.

#include "Modes.h"
#include <iomanip>

void modeMenu()
{
	std::cout << "All of the snow simulation modes and the key that should be pressed to select them are as follows:\n\n"
		"Standard snowfall ------ s\n"
		"Wind ------------------- + or - to increase or decrease wind\n"
		"Votex ------------------ v\n"
		"Perlin Noise Generator - p\n";
}

void perlinMode(Field& field)
{
	Field gradients(24, 24, 24);
	gradients.genGradients();
	field.addPerlin(gradients);
}

void vortexMode(Field& field)
{
	field.setField(Vec3f(-0.5f, -1.0f, -1.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
	field.setField(Vec3f(-1.0f, -1.0f, 0.5f), Vec3f(0.0f, 0.0f, -1.0f), Vec3f(1.0f, 1.0f, 0.0f));
	field.setField(Vec3f(0.5f, -1.0f, 1.0f), Vec3f(-1.0f, 0.0f, -1.0f), Vec3f(0.0f, 1.0f, 0.0f));
	field.setField(Vec3f(1.0f, -1.0f, -0.5f), Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 1.0f));

	field.setField(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(1.0f, 0.0f, 1.0f));
}

void checkInput(GLFWwindow* window, Field& forceField, Field& baseField, bool& perlin)
{
	enum button
	{
		KEY_S,
		KEY_EQUAL,
		KEY_MINUS,
		KEY_V,
		KEY_P
	};

	std::vector<int> pressed(5, 0);
	// Mode switching
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && pressed[KEY_S] == 0)
	{
		forceField.setField(Vec3f(0, -0.5, 0), Vec3f(-1, -1, -1), Vec3f(1, 1, 1));
		perlin = false;
		baseField = forceField;
		pressed[KEY_S] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && pressed[KEY_S] == 1)
	{
		pressed[KEY_S] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && pressed[KEY_EQUAL] == 0)
	{
		pressed[KEY_EQUAL] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE && pressed[KEY_EQUAL] == 1)
	{
		forceField.setWind(Vec3f(0.1, 0, 0));
		baseField = forceField;
		pressed[KEY_EQUAL] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && pressed[KEY_MINUS] == 0)
	{
		pressed[KEY_MINUS] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE && pressed[KEY_MINUS] == 1)
	{
		forceField.setWind(Vec3f(-0.1, 0, 0));
		baseField = forceField;
		pressed[KEY_MINUS] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && pressed[KEY_V] == 0)
	{
		vortexMode(forceField);
		perlin = false;
		baseField = forceField;
		pressed[KEY_V] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE && pressed[KEY_V] == 1)
	{
		pressed[KEY_V] = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && pressed[KEY_P] == 0)
	{
		perlinMode(forceField);
		perlin = true;
		baseField = forceField;
		pressed[KEY_P] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE && pressed[KEY_P] == 1)
	{
		pressed[KEY_P] = 0;
	}
}