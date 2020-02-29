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
	Particle(float x, float y, float z);
	Particle(Vec3f nPos);

	std::vector<float> getPos() const;

	void setPos(float x, float y, float z);
	void setPos(Vec3f nPos);

	void setForce(float x, float y, float z);
	void setForce(Vec3f nForce);

private:

	Vec3f pos_;

	Vec3f force_;

};

#endif