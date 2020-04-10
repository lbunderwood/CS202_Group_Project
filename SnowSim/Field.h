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
	Vec3f getForce(float x, float y, float z) const;

	// get vector at position given by Vec3f
	Vec3f getForce(const Vec3f& vec) const;

	// sets wind speed for whole field
	void setWind(float windspeed);

	// sets wind speed for part of the field
	void setWind(float windspeed, float yMin, float yMax);

	// sets wind speed for whole field using a Vec3f
	void setWind(const Vec3f& wind);

	// sets wind speed for part of the field using a Vec3f
	void setWind(const Vec3f& wind, float yMin, float yMax);

	// sets wind speed for part of the field using Vec3f for wind and region
	void setWind(const Vec3f& wind, const Vec3f& minCorner, const Vec3f& maxCorner);

private:

	// our actual field data. Stored as (x,y)
	std::vector<std::vector<std::vector<Vec3f>>> field_;

};

#endif