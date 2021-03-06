// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Particle.cpp
// source for Particle class

#include "Particle.h"
#include <cmath>


Particle::Particle() {}


Particle::Particle(float x, float y, float z) {
    pos_.setVec(x, y, z);
	force_.setVec(0.0f, 0.0f, 0.0f);
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

void Particle::update(const double& dt, const Field& f)
{
	force_ = f.getForce(pos_);
	vel_ += ((float)dt * force_);
	pos_ += ((float)dt * vel_);
}


void Particle::pushData(std::vector<float>& vertices)
{
	vertices.push_back(pos_.x_);
	vertices.push_back(pos_.y_);
	vertices.push_back(pos_.z_);
}


bool Particle::checkBounds()
{
	if (abs(pos_.x_) > 1.0f ||
		abs(pos_.y_) > 1.0f ||
		abs(pos_.z_) > 1.0f) {
		return 1;
	}
	return 0;
}