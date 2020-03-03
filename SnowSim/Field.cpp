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
	for (int x = 0; x < 20; x++)
	{
		std::vector<Vec3f> TempVec(20, Vec3f(0, -9.81, 0));
	}
}

// get vector at position x, y using the opengl coordinate system
Vec3f Field::getForce(float x, float y) const
{
	return field_[floor((x + 1.0f) * 10.0f)][floor((y + 1.0f) * 10.0f)];
}

// sets wind speed for whole field
void Field::setWind(float windspeed)
{
	for (auto n : field_)
	{
		for (auto m : n)
		{
			auto temp = m.getVec();
			m.setVec(windspeed, temp[1], temp[2]);
		}
	}
}