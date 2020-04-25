// Created by Luke Underwood
// 4/25/2020
// SnowSim
// Modes.h
// Contains function prototypes to enable multiple field "modes" so that
// the user can dynamically switch between a number of different fields.

#ifndef MODES_H
#define MODES_H

#include "Field.h"
#include "Vec3f.h"
#include <iostream>

void perlinMode();

void vortexMode();

void standardMode();

void simpleWind();

#endif