// Luke Underwood and Andrew Ng
// 3/3/2020
// CS202_Group_Project
// SnowSim
// Field.h
// header for Field class

#include<vector>
#include"Vec3f.h"

#ifndef FIELD_H
#define FIELD_H

class Field
{
public:

	// Default constructor. Sets everything to only gravity
	Field();

	// get vector at position x, y
	Vec3f getForce(float x, float y) const;

	// sets wind speed for whole field
	void setWind(float windspeed);

private:

	// our actual field data. Stored as (x,y)
	std::vector<std::vector<Vec3f>> field_;

};

#endif