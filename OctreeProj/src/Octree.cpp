#include "Octree.h"

Octree::Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &density)
{
	_origin = origin;
	_hDimention = hDimension;
	_box = NULL;
	_density = density;
	_level = level;
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
	_box->render();
	for (int i = 0; i < 8; i++)
	{
		if (_children[i] != NULL)
			_children[i]->renderBox();
	}
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
	//cout << endl <<  "LEVEL: " << _level << endl;
	//cout << "TRIANGLES SIZE: " << _triangles.size() << endl << endl;
	if ((_level + 1 < 8))
	{
		int density = 0;
		for (int i = 0; i < 8; i++)
		{
			density = (trianglesPerOctant[i].size() * _density) / _triangles.size();
			/*cout << "TRIANGLES PER OCTANT: " << i << " is : " << trianglesPerOctant[i].size() << endl;
			cout << "density " << density << endl;*/
			if (density >= 1)
			{
				//cout << "NEW CHILD" << endl;
				vector3f newOrigin = _origin;
				newOrigin.x += _hDimention.x / (i & 0x04 ? 2.0 : -2.0);
				newOrigin.y += _hDimention.y / (i & 0x02 ? 2.0 : -2.0);
				newOrigin.z += _hDimention.z / (i & 0x01 ? 2.0 : -2.0);
				_children[i] = new Octree(newOrigin, _hDimention / 2.0, _level + 1, density);
				_children[i]->insert(trianglesPerOctant[i]);
				_children[i]->createCBox(_origin + _hDimention, _origin - _hDimention);
				_children[i]->makeOctree(vertices);
			}
				
		}
	}
	//cout << endl << "Volvemos al padre" << endl;
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