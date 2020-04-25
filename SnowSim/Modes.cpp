// Created by Luke Underwood
// 4/25/2020
// SnowSim
// Modes.cpp
// Contains function definitions to enable multiple field "modes" so that
// the user can dynamically switch between a number of different fields.

#include "Modes.h"

void perlinMode(Field& field)
{
	Field gradients(24, 24, 24);
	gradients.genGradients();
	field.addPerlin(gradients);
}

void vortexMode(Field& field)
{

}

void standardMode(Field& field)
{

}

void simpleWind(Field& field, int speed)
{

}