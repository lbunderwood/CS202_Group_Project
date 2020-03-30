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
	force_.setVec(0.0f, -9.8f, 0.0f);
}


Particle::Particle(Vec3f nPos) : pos_(nPos) {}


std::vector<float> Particle::getPos() const
{
    std::vector<float> posv = pos_.getVec();
	std::vector<float> output = { posv[0], posv[1], posv[2] };
	return output;
}

std::vector<float> Particle::getVel() const
{
	std::vector<float> velv = vel_.getVec();
	std::vector<float> output = { velv[0], velv[1], velv[2] };
	return output;
}

std::vector<float> Particle::getForce() const
{
	std::vector<float> accv = force_.getVec();
	std::vector<float> output = { accv[0], accv[1], accv[2] };
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

void Particle::setVel(float x, float y, float z)
{
	vel_.setVec(x, y, z);
}

void Particle::setVel(Vec3f nVel)
{
	vel_ = nVel;
}

void Particle::setForce(float x, float y, float z)
{
    force_.setVec(x, y, z);
}

void Particle::setForce(Vec3f nForce)
{
    force_ = nForce;
}

void Particle::update(const double& dt)
{
	std::vector<float> tempf = force_.getVec();
	std::vector<float> tempv = vel_.getVec();
	std::vector<float> tempp = pos_.getVec();
	tempv[1] += (tempf[1] * dt);
	tempp[1] += (tempv[1] * dt);
	vel_.setVec(tempv[0], tempv[1], tempv[2]);
	pos_.setVec(tempp[0], tempp[1], tempp[2]);
}


void Particle::pushData(std::vector<float>& vertices)
{
	vertices.push_back(pos_.x_);
	vertices.push_back(pos_.y_);
	vertices.push_back(pos_.z_);
}