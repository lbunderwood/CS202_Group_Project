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
	field.setWind(Vec3f(-0.5f, 0.0f, -1.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
	field.setWind(Vec3f(-1.0f, 0.0f, 0.5f), Vec3f(0.0f, 0.0f, -1.0f), Vec3f(1.0f, 1.0f, 0.0f));
	field.setWind(Vec3f(0.5f, 0.0f, 1.0f), Vec3f(-1.0f, 0.0f, -1.0f), Vec3f(0.0f, 1.0f, 0.0f));
	field.setWind(Vec3f(1.0f, 0.0f, -0.5f), Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 1.0f));

	field.setWind(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(1.0f, 0.0f, 1.0f));
}