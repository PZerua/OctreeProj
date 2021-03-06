#pragma once

#include "header.h"
#include "vector3f.h"
#include "ray.h"

class ContainerBox
{
public:
	ContainerBox();
	ContainerBox(const vector3f &minCorner, const vector3f &maxCorner);
	~ContainerBox();
	void render(vector3f color);
	void setMinMax(const vector3f &minCorner, const vector3f &maxCorner);
	vector3f getMin();
	vector3f getMax();
private:
	vector3f _minCorner;
	vector3f _maxCorner;
};
