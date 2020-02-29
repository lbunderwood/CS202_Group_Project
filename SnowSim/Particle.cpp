// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Particle.cpp
// source for Particle class

#include "Particle.h"


Particle::Particle() {}


Particle::Particle(float x, float y, float z) {
    pos_.setVec(x, y, z);
}


Particle::Particle(Vec3f nPos) : pos_(nPos) {}


std::vector<float> Particle::getPos() const
{
    std::vector<float> posv = pos_.getVec();
	std::vector<float> output = { posv[0], posv[1], posv[2] };
	return output;
}


void Particle::setPos(float x, float y, float z)
{
    pos_.setVec(x, y, z);
}

void Particle::setPos(Vec3f nPos)
{
    pos_ = nPos;
}

void Particle::setForce(float x, float y, float z)
{
    force_.setVec(x, y, z);
}

void Particle::setForce(Vec3f nForce)
{
    force_ = nForce;
}