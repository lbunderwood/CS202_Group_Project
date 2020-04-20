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
	field_ = std::vector<std::vector<std::vector<Vec3f>>>(21, std::vector<std::vector<Vec3f>>(21, std::vector<Vec3f>(21, Vec3f(0, -0.5, 0))));
}

// Constructor for custom sizes
Field::Field(int x, int y, int z)
{
	field_ = std::vector<std::vector<std::vector<Vec3f>>>(x, std::vector<std::vector<Vec3f>>(y, std::vector<Vec3f>(z, Vec3f(0, -0.5, 0))));
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

// sets wind speed for whole field in the X direction only
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

// sets wind speed for whole field using a Vec3f
void Field::setWind(const Vec3f& wind)
{
	for (auto& n : field_)
	{
		for (auto& m : n)
		{
			for (auto& l : m)
			{
				l += wind;
			}
		}
	}
}

// I don't like these functions but they keep the next two dry
void roundUp(float& num)
{
	if (num < -1)
	{
		num = -1;
	}
}
void roundDown(float& num)
{
	if (num > 1)
	{
		num = 1;
	}
}

// sets wind speed for part of the field using Vec3f for wind and region
void Field::setWind(const Vec3f& wind, const Vec3f& minCorner, const Vec3f& maxCorner)
{
	Vec3f min = minCorner;
	Vec3f max = maxCorner;

	roundUp(min.x_);
	roundUp(min.y_);
	roundUp(min.z_);
	roundDown(max.x_);
	roundDown(max.y_);
	roundDown(max.z_);

	for (int i = floor(((double)min.x_ + 1.0) * 10.0); i < floor(((double)max.x_ + 1.0) * 10.0); i++)
	{
		for (int j = floor(((double)min.y_ + 1.0) * 10.0); j < floor(((double)max.y_ + 1.0) * 10.0); j++)
		{
			for (int k = floor(((double)min.z_ + 1.0) * 10.0); k < floor(((double)max.z_ + 1.0) * 10.0); k++)
			{
				field_[i][j][k] += wind;
			}
		}
	}
}

// sets field value for part of the field using Vec3f for wind and region
void Field::setField(const Vec3f& vector, const Vec3f& minCorner, const Vec3f& maxCorner)
{
	Vec3f min = minCorner;
	Vec3f max = maxCorner;

	roundUp(min.x_);
	roundUp(min.y_);
	roundUp(min.z_);
	roundDown(max.x_);
	roundDown(max.y_);
	roundDown(max.z_);

	for (int i = floor(((double)min.x_ + 1.0) * 10.0); i < floor(((double)max.x_ + 1.0) * 10.0); i++)
	{
		for (int j = floor(((double)min.y_ + 1.0) * 10.0); j < floor(((double)max.y_ + 1.0) * 10.0); j++)
		{
			for (int k = floor(((double)min.z_ + 1.0) * 10.0); k < floor(((double)max.z_ + 1.0) * 10.0); k++)
			{
				field_[i][j][k] = vector;
			}
		}
	}
}

// generates random unit vectors for use in Perlin noise
void Field::genGradients()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	for (auto& n : field_)
	{
		for (auto& m : n)
		{
			for (auto& l : m)
			{
				l = Vec3f(dist(gen), dist(gen), dist(gen)).unit();
			}
		}
	}
}

// adds noise vectors to current Field based on values from Perlin function
void Field::addPerlin(const Field& g)
{
	for (int k = 0; k < field_.size(); k++)
	{
		for (int j = 0; j < field_[k].size(); j++)
		{
			for (int i = 0; i < field_[k][j].size(); i++)
			{
				float v0 = 5.0f * perlin(i + 0.5f, j + 0.5f, k + 0.5f, g);
				float v1 = 5.0f * perlin(i + 1.5f, j + 1.5f, k + 1.5f, g);
				float v2 = 5.0f * perlin(i + 2.5f, j + 2.5f, k + 2.5f, g);
				field_[k][j][i] += Vec3f(v0, v1, v2);
			}
		}
	}
}

// linear interpolation function (w is weight)
float lerp(float a, float b, float w)
{
	return ((1.0f - w) * a) + (w * b);
}

// returns dot product of distance vector and gradient for Perlin function
float dotDistGradient(int ix, int iy, int iz, float x, float y, float z, const Field& g)
{
	Vec3f d(x - ix, y - iy, z - iz);

	return (d.dot(g.getForce((float)ix / 10.0f - 1.0f, (float)iy / 10.0f - 1.0f, (float)iz / 10.0f - 1.0f)));
}

// Perlin noise function
float perlin(float x, float y, float z, const Field& g)
{
	int x0 = floor(x);
	int x1 = x0 + 1;
	int y0 = floor(y);
	int y1 = y0 + 1;
	int z0 = floor(z);
	int z1 = z0 + 1;

	float wx = x - x0;
	float wy = y - y0;
	float wz = z - z0;

	float n0, n1, ix0, ix1, iy0, iy1, value;

	n0 = dotDistGradient(x0, y0, z0, x, y, z, g);
	n1 = dotDistGradient(x1, y0, z0, x, y, z, g);
	ix0 = lerp(n0, n1, wx);

	n0 = dotDistGradient(x0, y1, z0, x, y, z, g);
	n1 = dotDistGradient(x1, y1, z0, x, y, z, g);
	ix1 = lerp(n0, n1, wx);

	iy0 = lerp(ix0, ix1, wy);

	n0 = dotDistGradient(x0, y0, z1, x, y, z, g);
	n1 = dotDistGradient(x1, y0, z1, x, y, z, g);
	ix0 = lerp(n0, n1, wx);

	n0 = dotDistGradient(x0, y1, z1, x, y, z, g);
	n1 = dotDistGradient(x1, y1, z1, x, y, z, g);
	ix1 = lerp(n0, n1, wx);

	iy1 = lerp(ix0, ix1, wy);

	value = lerp(iy0, iy1, wz);

	return value;
}