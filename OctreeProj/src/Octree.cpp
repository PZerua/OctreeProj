#include "Octree.h"

Octree::Octree(const vector3f &origin, const vector3f &hDimension)
{
	_origin = origin;
	_hDimention = hDimension;
	_box = NULL;
	for (int i = 0; i < 8; i++)
	{
		_children[i] = NULL;
	}
}

bool Octree::isLeaf()
{
	for (int i = 0; i < 8; i++)
		if (_children[i] != NULL)
			return false;
	return true;
}

void Octree::insert(triangle *triangles)
{
	_triangles = triangles;

	/*
	cout << _data[0].a << " " << _data[0].b << " " << _data[0].c << endl;
	cout << _data[1].a << " " << _data[1].b << " " << _data[1].c << endl;
	cout << _data[2].a << " " << _data[2].b << " " << _data[2].c << endl;
	*/
}

void Octree::renderBox()
{
	if (_box != NULL)
		_box->render();
}

void Octree::createCBox(const vector3f &minCorner, const vector3f &maxCorner)
{
	_box = new ContainerBox(minCorner, maxCorner);
}

void Octree::makeOctree(vector3f *vertices) 
{
	// TODO
}
