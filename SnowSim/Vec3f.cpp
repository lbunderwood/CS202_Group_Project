// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Vec3f.cpp
// source for Vec3f class

#include "Vec3f.h"

// Default Constructor
Vec3f::Vec3f() : x_(0.0f), y_(0.0f), z_(0.0f) {}

// Float-valued constructor
Vec3f::Vec3f(float x, float y, float z) : x_(x), y_(y), z_(z) {}

// returns vec of the values
std::vector<float> Vec3f::getVec() const
{
	return { x_, y_, z_ };
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
	return x_ * vec.x_ + y_ * vec.y_ + z_ * vec.z_;
}

// returns result of dot operation
Vec3f Vec3f::cross(const Vec3f& vec) const
{
	return Vec3f(y_ * vec.z_ - vec.y_ * z_,
				 z_ * vec.x_ - vec.z_ * x_,
				 x_ * vec.y_ - vec.x_ * y_);
}

// returns length of vector
float Vec3f::length() const
{
	return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
}

// returns the unit vector
Vec3f Vec3f::unit() const
{
	return (1 / length()) * *this;
}


//////////////////////////////////////////////
////////	Operator Overloads	//////////////
//////////////////////////////////////////////

// prototype for vector addition operator
Vec3f operator +(const Vec3f& vec1, const Vec3f& vec2)
{
	return Vec3f(vec1.x_ + vec2.x_, vec1.y_ + vec2.y_, vec1.z_ + vec2.z_);
}

// prototype for vector subtraction operator
Vec3f operator -(const Vec3f& vec1, const Vec3f& vec2)
{
	return Vec3f(vec1.x_ - vec2.x_, vec1.y_ - vec2.y_, vec1.z_ - vec2.z_);
}

// prototype for scalar multiplication by float operator
Vec3f operator *(float num, const Vec3f& vec)
{
	return Vec3f(vec.x_ * num, vec.y_ * num, vec.z_ * num);
}

// prototype for scalar multiplication by int operator
Vec3f operator *(int num, const Vec3f& vec)
{
	return Vec3f(vec.x_ * num, vec.y_ * num, vec.z_ * num);
}

// prototype for vector comparison ==
bool operator ==(const Vec3f& vec1, const Vec3f& vec2)
{
	return vec1.x_ == vec2.x_ &&
		   vec1.y_ == vec2.y_ &&
		   vec1.z_ == vec2.z_;
}

// prototype for vector comparison !=
bool operator !=(const Vec3f& vec1, const Vec3f& vec2)
{
	return  vec1.x_ != vec2.x_ ||
			vec1.y_ != vec2.y_ ||
			vec1.z_ != vec2.z_;
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

// definition for compound assignment operator +=
Vec3f& Vec3f::operator+=(const Vec3f& oldVec)
{
	*this = oldVec + *this;
	return *this;
}

// definition for compound assignment operator -=
Vec3f& Vec3f::operator-=(const Vec3f& oldVec)
{
	*this = oldVec - *this;
	return *this;
}