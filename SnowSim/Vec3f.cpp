// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Vec3f.cpp
// source for Vec3f class

#include "Vec3f.h"

// Default Constructor
Vec3f::Vec3f() : x_(0), y_(0), z_(0) {}

// Float-valued constructor
Vec3f::Vec3f(float x, float y, float z) : x_(x), y_(y), z_(z) {}

// Int-valued constructor
Vec3f::Vec3f(int x, int y, int z) : x_(x), y_(y), z_(z) {}

// returns vec of the values
std::vector<float> Vec3f::getVec() const
{
	std::vector<float> output = { x_, y_, z_ };
	return output;
}

// sets new vector values with floats
void Vec3f::setVec(float x, float y, float z)
{
	x_ = x;
	y_ = y;
	z_ = z;
}

// sets new vector values with ints
void Vec3f::setVec(int x, int y, int z)
{
	x_ = x;
	y_ = y;
	z_ = z;
}

// returns result of dot operation
float Vec3f::dot(const Vec3f& vec) const
{
	std::vector<float> temp = vec.getVec();
	return x_ * temp[0] + y_ * temp[1] + z_ * temp[2];
}

// returns result of dot operation
Vec3f Vec3f::cross(const Vec3f& vec) const
{
	std::vector<float> temp = vec.getVec();
	Vec3f output(
		y_ * temp[2] - temp[1] * z_,
		z_ * temp[0] - temp[2] * x_,
		x_ * temp[1] - temp[0] * y_);
	return output;
}

// returns length of vector
float Vec3f::length() const
{
	return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
}

// returns the unit vector
Vec3f Vec3f::unit() const
{
	Vec3f output = (1 / length()) * *this;
	return output;
}


//////////////////////////////////////////////
////////	Operator Overloads	//////////////
//////////////////////////////////////////////

// prototype for vector addition operator
Vec3f operator +(const Vec3f& vec1, const Vec3f& vec2)
{
	std::vector<float> temp1 = vec1.getVec();
	std::vector<float> temp2 = vec2.getVec();
	Vec3f output(
		temp1[0] + temp2[0],
		temp1[1] + temp2[1],
		temp1[2] + temp2[2]);
	return output;
}

// prototype for vector subtraction operator
Vec3f operator -(const Vec3f& vec1, const Vec3f& vec2)
{
	std::vector<float> temp1 = vec1.getVec();
	std::vector<float> temp2 = vec2.getVec();
	Vec3f output(
		temp1[0] - temp2[0],
		temp1[1] - temp2[1],
		temp1[2] - temp2[2]);
	return output;
}

// prototype for scalar multiplication by float operator
Vec3f operator *(float num, const Vec3f& vec)
{
	std::vector<float> temp = vec.getVec();
	Vec3f output(
		temp[0] * num,
		temp[1] * num,
		temp[2] * num);
	return output;
}

// prototype for scalar multiplication by int operator
Vec3f operator *(int num, const Vec3f& vec)
{
	std::vector<float> temp = vec.getVec();
	Vec3f output(
		temp[0] * num,
		temp[1] * num,
		temp[2] * num);
	return output;
}

// prototype for vector comparison ==
bool operator ==(const Vec3f& vec1, const Vec3f& vec2)
{
	std::vector<float> temp1 = vec1.getVec();
	std::vector<float> temp2 = vec2.getVec();
	return temp1[0] == temp2[0] &&
		   temp1[1] == temp2[1] &&
		   temp1[2] == temp2[2];
}

// prototype for vector comparison !=
bool operator !=(const Vec3f& vec1, const Vec3f& vec2)
{
	std::vector<float> temp1 = vec1.getVec();
	std::vector<float> temp2 = vec2.getVec();
	return temp1[0] != temp2[0] ||
		   temp1[1] != temp2[1] ||
		   temp1[2] != temp2[2];
}

// prototype for vector comparison strictly greater than
bool operator >(const Vec3f& vec1, const Vec3f& vec2)
{
	return vec1.length() > vec2.length();
}

// prototype for vector comparison strictly less than
bool operator <(const Vec3f& vec1, const Vec3f& vec2)
{
	return vec1.length() < vec2.length();
}

// prototype for vector comparison less than or equal to
bool operator <=(const Vec3f& vec1, const Vec3f& vec2)
{
	return vec1.length() <= vec2.length();
}

// prototype for vector comparison greater than or equal to
bool operator >=(const Vec3f& vec1, const Vec3f& vec2)
{
	return vec1.length() >= vec2.length();
}