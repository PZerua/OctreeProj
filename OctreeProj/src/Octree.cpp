#include "Octree.h"

vector3f Octree::_intersection = vector3f(-1, -1, -1);

Octree::Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &density, Octree *pointerToFather)
{
	_origin = origin;
	_hDimention = hDimension;
	_box = NULL;
	_density = density;
	_level = level;
	_pointerToFather = pointerToFather;

	for (int i = 0; i < 8; i++)
	{
		_children[i] = NULL;
	}
	_foundInter = false;
}

Octree::~Octree()
{
	for (int i = 0; i < 8; i++)
		delete _children[i];
}

bool Octree::isLeaf()
{
	for (int i = 0; i < 8; i++)
		if (_children[i] != NULL)
			return false;
	return true;
}

void Octree::insert(vector<triangle *> triangles)
{
	_triangles = triangles;

	/*
	cout << _data[0].a << " " << _data[0].b << " " << _data[0].c << endl;
	cout << _data[1].a << " " << _data[1].b << " " << _data[1].c << endl;
	cout << _data[2].a << " " << _data[2].b << " " << _data[2].c << endl;
	*/
}

void Octree::renderBox(vector3f color, Ray &ray)
{
	_box->render(color, ray);
}

void Octree::createCBox(const vector3f &minCorner, const vector3f &maxCorner)
{
	_box = new ContainerBox(minCorner, maxCorner);
}

void Octree::makeOctree(const vector<vector3f> &vertices)
{
	vector<triangle *> trianglesPerOctant[8];

	for (unsigned i = 0; i < _triangles.size(); i++)
	{
		vector3f firstVertex = vertices[_triangles[i]->a];
		int position = getOctant(firstVertex);
		trianglesPerOctant[position].push_back(_triangles[i]);
	}
	if ((_level + 1 < 8))
	{
		int density = 0;
		for (int i = 0; i < 8; i++)
		{
			density = (trianglesPerOctant[i].size() * _density) / _triangles.size();
			if (density >= 1)
			{
				vector3f newOrigin = _origin;
				newOrigin.x += _hDimention.x / (i & 0x04 ? 2.0 : -2.0);
				newOrigin.y += _hDimention.y / (i & 0x02 ? 2.0 : -2.0);
				newOrigin.z += _hDimention.z / (i & 0x01 ? 2.0 : -2.0);
				_children[i] = new Octree(newOrigin, _hDimention / 2.0, _level + 1, density, this);
				_children[i]->insert(trianglesPerOctant[i]);
				_children[i]->createCBox(newOrigin + _hDimention / 2.0, newOrigin - _hDimention / 2.0);
				_children[i]->makeOctree(vertices);
			}
		}
	}
}

short Octree::getOctant(const vector3f &vertex)
{
	short position = 0;
	if (vertex.x >= _origin.x) position |= 0x04;
	if (vertex.y >= _origin.y) position |= 0x02;
	if (vertex.z >= _origin.z) position |= 0x01;
	return position;
}


Octree * Octree::getChild(int i) 
{
	return _children[i];
}

int Octree::getLevel()
{
	return _level;
}

vector<triangle *> Octree::getTriangles()
{
	return _triangles;
}

Octree *Octree::getPointerToFather()
{
	return _pointerToFather;
}

bool Octree::isIntersection(Ray &ray, const vector<vector3f> &vertices)
{
	if (!isLeaf())
	{
		_foundInter = false;
		for (int i = 0; i < 8; i++)
		{
			if (_children[i] != NULL)
				_foundInter = _children[i]->isIntersection(ray, vertices);
			if (_foundInter)
				break;
		}
	}
	else
	{
		for (unsigned i = 0; i < _triangles.size(); i++)
		{
			vector3f v0 = vertices[_triangles[i]->a];
			vector3f v1 = vertices[_triangles[i]->b];
			vector3f v2 = vertices[_triangles[i]->c];
			vector3f point = ray.origin;
			vector3f dir = ray.end - ray.origin;
			_foundInter = isIntersectionTri(v0, v1, v2, point, dir);
			if (_foundInter)
			{
				_intersection = vector3f(_triangles[i]->a, _triangles[i]->b, _triangles[i]->c);
				return _foundInter;
			}
			else _intersection = vector3f(-1, -1, -1);
				
		}
		return false;
	}
	return _foundInter;
}

bool Octree::isIntersectionTri(vector3f &v0, vector3f &v1, vector3f &v2, vector3f &point, vector3f &dir)
{
	float a, f, u, v, t;

	vector3f e1 = v1 - v0;
	vector3f e2 = v2 - v0;

	vector3f h = crossProduct(dir, e2);
	a = dotProduct(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1 / a;
	vector3f s = point - v0;
	u = f * (dotProduct(s, h));

	if (u < 0.0 || u > 1.0)
		return(false);

	vector3f q = crossProduct(s, e1);
	v = f * dotProduct(dir, q);

	if (v < 0.0 || u + v > 1.0)
		return(false);

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * dotProduct(e2, q);

	if (t > 0.00001) // ray intersection
		return(true);

	else // this means that there is a line intersection
		 // but not a ray intersection
		return (false);
}

vector3f &Octree::getTriangInter()
{
	return _intersection;
}