// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Vec3f.h
// header for Vec3f class

#include<vector>

#ifndef VEC3F_H
#define VEC3F_H

class Vec3f
{
public:
	// Constructors
	Vec3f();
	Vec3f(float x, float y, float z);
	Vec3f(int x, int y, int z);

	// returns vec of the values
	std::vector<float> getVec() const;

	// sets new vector values
	void setVec(float x, float y, float z);
	void setVec(int x, int y, int z);

	// returns result of dot operation
	float dot(const Vec3f& vec) const;

	// returns result of dot operation
	Vec3f cross(const Vec3f& vec) const;

	// returns length of vector
	float length() const;

	// returns the unit vector
	Vec3f unit() const;

	// compound assignment operator prototypes
	Vec3f& operator+=(const Vec3f& oldVec);
	Vec3f& operator-=(const Vec3f& oldVec);

//private:

	float x_;
	float y_;
	float z_;
};

// prototype for vector addition operator
Vec3f operator +(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector subtraction operator
Vec3f operator -(const Vec3f& vec1, const Vec3f& vec2);

// prototype for scalar multiplication by float operator
Vec3f operator *(float num, const Vec3f& vec2);

// prototype for scalar multiplication by int operator
Vec3f operator *(int num, const Vec3f& vec2);

// prototype for vector comparison ==
bool operator ==(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector comparison !=
bool operator !=(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector comparison strictly greater than
bool operator >(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector comparison strictly less than
bool operator <(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector comparison less than or equal to
bool operator <=(const Vec3f& vec1, const Vec3f& vec2);

// prototype for vector comparison greater than or equal to
bool operator >=(const Vec3f& vec1, const Vec3f& vec2);

#endif