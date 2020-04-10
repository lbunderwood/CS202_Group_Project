// Luke Underwood and Andrew Ng
// 3/3/2020
// CS202_Group_Project
// SnowSim
// Field.cpp
// source for Field class

#include"Field.h"
#include"Vec3f.h"

// Default constructor. Sets everything to only gravity
Field::Field()
{
	field_ = std::vector<std::vector<std::vector<Vec3f>>>(21, std::vector<std::vector<Vec3f>>(21, std::vector<Vec3f>(21, Vec3f(0, -0.5f, 0))));
}

// get vector at position x, y, z using the opengl coordinate system
Vec3f Field::getForce(float x, float y, float z) const
{
	return field_[floor(((double)x + 1.0) * 10.0)][floor(((double)y + 1.0) * 10.0)][floor(((double)z + 1.0) * 10.0)];
}

// get vector at position given by Vec3f
Vec3f Field::getForce(const Vec3f& vec) const
{
	return field_[floor(((double)vec.x_ + 1.0) * 10.0)][floor(((double)vec.y_ + 1.0) * 10.0)][floor(((double)vec.z_ + 1.0) * 10.0)];
}

// sets wind speed for whole field
void Field::setWind(float windspeed)
{
	for (auto& n : field_)
	{
		for (auto& m : n)
		{
			for (auto& l : m)
			{
				l.x_ = windspeed;
			}
		}
	}
}

// sets wind speed for part of the field
void Field::setWind(float windspeed, float yMin, float yMax)
{
	for (auto& n : field_)
	{
		for (int i = floor(((double)yMin + 1.0) * 10.0); i < floor(((double)yMax + 1.0) * 10.0); i++)
		{
			for (auto& m : n[i])
			{
				m.x_ = windspeed;
			}
		}
	}
	
}