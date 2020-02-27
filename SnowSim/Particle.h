// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Particle.h
// header for Particle class

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec3f.h"

class Particle
{
public:

	Particle();

private:

	Vec3f pos;

	Vec3f force;

};

#endif