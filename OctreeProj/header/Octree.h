#pragma once
#include "header.h"
#include "vector3f.h"
#include "ContainerBox.h"

class Octree
{
public:
	Octree(const vector3f &origin, const vector3f &hDimension);
	~Octree();
	int getBoxContainsPoint(const vector3f &point);
	bool isLeaf();
	void insert(vector3f *data);
	void split();
	void createCBox(const vector3f &minCorner, const vector3f &maxCorner);
	void renderBox();

private:
	vector3f _origin;
	vector3f _hDimention;
	vector3f *_data;
	Octree *_children[8];
	ContainerBox *_box;
};