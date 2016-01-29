#pragma once
#include "header.h"
#include "vector3f.h"
#include "ContainerBox.h"

class Octree
{
public:
	Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &density);
	~Octree();
	int getBoxContainsPoint(const vector3f &point);
	bool isLeaf();
	void insert(vector<triangle> triangles);
	void createCBox(const vector3f &minCorner, const vector3f &maxCorner);
	void renderBox(vector3f color);
	void makeOctree(const vector<vector3f> &vertices);
	short getOctant(const vector3f &vertex);
	Octree * getChild(int i);
	

private:
	vector3f _origin;
	vector3f _hDimention;
	vector<triangle> _triangles;
	Octree *_children[8];
	ContainerBox *_box;
	int _level;
	int _density;
};