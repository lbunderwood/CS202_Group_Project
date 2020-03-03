// Luke Underwood and Andrew Ng
// 3/3/2020
// CS202_Group_Project
// SnowSim
// Field.cpp
// source for Field class

#include"Field.h"
#include"Vec3f.h"

Field::Field()
{
	for (int x = 0; x < 20; x++)
	{
		std::vector<Vec3f> TempVec(20, Vec3f(0, -9.81, 0));
	}
}