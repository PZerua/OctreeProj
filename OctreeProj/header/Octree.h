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
	void insert(triangle *triangles);
	void split();
	void createCBox(const vector3f &minCorner, const vector3f &maxCorner);
	void renderBox();
	void makeOctree(vector3f *vertices);

private:
	vector3f _origin;
	vector3f _hDimention;
	triangle *_triangles;
	Octree *_children[8];
	ContainerBox *_box;
};