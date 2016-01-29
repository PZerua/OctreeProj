#include "Octree.h"

Octree::Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &maxTriangles, const int &density)
{
	_origin = origin;
	_hDimention = hDimension;
	_box = NULL;
	_density = density;
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

void Octree::insert(vector<triangle> triangles)
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

void Octree::makeOctree(const vector<vector3f> &vertices)
{
	vector<triangle> trianglesPerOctant[8];

	for (int i = 0; i < _triangles.size(); i++)
	{
		vector3f firstVertex = vertices[_triangles[i].a];
		int position = getOctant(firstVertex);
		trianglesPerOctant[position].push_back(_triangles[i]);
	}
	cout << "LEVEL: " << _level << endl;
	cout << "TRIANGLES SIZE: " << _triangles.size() << endl;
	if ((_level + 1 < 8))
	{
		int density = 0;
		for (int i = 0; i < 8; i++)
		{
			density = (trianglesPerOctant[i].size() * _density) / _triangles.size();
			cout << "TRIANGLES PER OCTANT: " << i << " is : " << trianglesPerOctant[i].size() << endl;
			cout << "density " << (int)density << endl;
			if (density >= 5)
			{
				//_children[i] = new Octree()
			}
				
		}
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

short Octree::getOctant(const vector3f &vertex)
{
	short position = 0;
	if (vertex.x >= _origin.x) position |= 0x04;
	if (vertex.y >= _origin.y) position |= 0x02;
	if (vertex.z >= _origin.z) position |= 0x01;
	return position;
}