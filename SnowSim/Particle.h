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
	// Constructors
	Particle();
	Particle(float x, float y, float z);
	Particle(Vec3f nPos);

	// returns vec of pos values
	std::vector<float> getPos() const;

	// returns vec of vel values
	std::vector<float> getVel() const;

	// returns vec of force values
	std::vector<float> getForce() const;

	// sets new pos values
	void setPos(float x, float y, float z);
	void setPos(Vec3f nPos);

	// sets new vel values
	void setVel(float x, float y, float z);
	void setVel(Vec3f nVel);

	// sets new force values
	void setForce(float x, float y, float z);
	void setForce(Vec3f nForce);

	void update(const double& dt);

private:

	Vec3f pos_;

	Vec3f vel_;

	Vec3f force_;

};

#endif