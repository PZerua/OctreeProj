#pragma once
#include "header.h"
#include "vector3f.h"
#include "ContainerBox.h"
#include "ray.h"

class Octree
{
public:
	Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &density, Octree *pointerToFather);
	~Octree();
	bool isIntersection(Ray &ray, const vector<vector3f> &vertices);
	bool isIntersectionTri(vector3f &v0, vector3f &v1, vector3f &v2, vector3f &point, vector3f &dir);
	bool isLeaf();
	void insert(vector<triangle *> triangles);
	void createCBox(const vector3f &minCorner, const vector3f &maxCorner);
	void renderBox(vector3f color, Ray &ray);
	void makeOctree(const vector<vector3f> &vertices);
	short getOctant(const vector3f &vertex);
	Octree * getChild(int i);
	int getLevel();
	vector<triangle *> getTriangles();
	Octree *getPointerToFather();
	vector3f &getTriangInter();

private:
	vector3f _origin;
	vector3f _hDimention;
	vector<triangle *> _triangles;
	Octree *_children[8];
	ContainerBox *_box;
	int _level;
	int _density;
	Octree *_pointerToFather;
	bool _foundInter;
	static vector3f _intersection;
};