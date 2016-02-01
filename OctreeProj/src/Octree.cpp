#include "Octree.h"


// This class manage an octree and its nodes (in this case 8 children for each)
vector3f Octree::_intersection = vector3f(-1, -1, -1);

// Total of triangles checked
int Octree::_trianglesChecked = 0;

// Total of nodes checked
int Octree::_nodesChecked = 0;

Octree::Octree(const vector3f &origin, const vector3f &hDimension, const int &level, const int &density, Octree *pointerToFather)
{
	_origin = origin;
	// Distance from the origin to a side
	_hDimention = hDimension;
	// The box
	_box = NULL;
	// The density (percentage of triangles)
	_density = density;
	// The level of this node
	_level = level;
	// A pointer to the father to come back in the keyboard
	_pointerToFather = pointerToFather;

	// Initialize to null the children
	for (int i = 0; i < 8; i++)
	{
		_children[i] = NULL;
	}
	// Variable to know when a ray has intersected
	_foundInter = false;
}

Octree::~Octree()
{
	for (int i = 0; i < 8; i++)
		delete _children[i];
}

bool Octree::isLeaf()
{
	// We only create a children if a triangle is inserted, so if all
	// the children are NULL, this node is a leaf
	for (int i = 0; i < 8; i++)
		if (_children[i] != NULL)
			return false;
	return true;
}

// Insert a vector of pointers to triangles
void Octree::insert(vector<triangle *> triangles)
{
	_triangles = triangles;
}

// Render the box of this node (the bounding box if the node is on level 0)
void Octree::renderBox(vector3f color)
{
	_box->render(color);
}

// Creates the box from the min and max corner
void Octree::createCBox(const vector3f &minCorner, const vector3f &maxCorner)
{
	_box = new ContainerBox(minCorner, maxCorner);
}

// Iterative function to create children to spread vectors of triangles
void Octree::makeOctree(const vector<vector3f> &vertices)
{
	// Vector used to divide triangles depending on the octant they are
	vector<triangle *> trianglesPerOctant[8];

	for (unsigned i = 0; i < _triangles.size(); i++)
	{
		// We check the position of the first vertex of the triangle
		vector3f firstVertex = vertices[_triangles[i]->a];
		// We return on which octant this vertex is
		int position = getOctant(firstVertex);
		// We add the triangle in the vector depending on the octant
		trianglesPerOctant[position].push_back(_triangles[i]);
	}
	// And stop condition, the maximum level of the octree is 8
	if ((_level + 1 < 8))
	{
		// Variable to track the percentage of vertices
		int density = 0;
		// For each child
		for (int i = 0; i < 8; i++)
		{
			density = (trianglesPerOctant[i].size() * _density) / _triangles.size();
			// If the number of triangles is greater than the 0.001 percent of the total
			// in this case 12000 * 0.001 = 12, so only a new child will be created if it
			// stores more than 12 triangles
			if (density >= 1)
			{
				// We calculate the new origin in base of the octant
				vector3f newOrigin = _origin;
				newOrigin.x += _hDimention.x / (i & 0x04 ? 2.0 : -2.0);
				newOrigin.y += _hDimention.y / (i & 0x02 ? 2.0 : -2.0);
				newOrigin.z += _hDimention.z / (i & 0x01 ? 2.0 : -2.0);
				// We create the child
				_children[i] = new Octree(newOrigin, _hDimention / 2.0, _level + 1, density, this);
				_children[i]->insert(trianglesPerOctant[i]);
				_children[i]->createCBox(newOrigin + _hDimention / 2.0, newOrigin - _hDimention / 2.0);
				_children[i]->makeOctree(vertices);
			}
		}
	}
}

// Calculates the octant of this vertex
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

// Checks if the ray is intersecting
bool Octree::isIntersection(Ray &ray, const vector<vector3f> &vertices)
{
	// Only checks if its a leaf
	if (!isLeaf())
	{
		_foundInter = false;
		// If its not a leaf then lets check its children
		for (int i = 0; i < 8; i++)
		{
			if (_children[i] != NULL)
			{
				_nodesChecked++;
				_foundInter = _children[i]->isIntersection(ray, vertices);
			}
			// Stop searching if intersection is found
			if (_foundInter)
				break;
		}
	}
	// If its a leaf
	else
	{
		// For each of the triangles in this node
		for (unsigned i = 0; i < _triangles.size(); i++)
		{
			_trianglesChecked++;
			// We check the intersection of the ray with the triangles
			vector3f v0 = vertices[_triangles[i]->a];
			vector3f v1 = vertices[_triangles[i]->b];
			vector3f v2 = vertices[_triangles[i]->c];
			vector3f point = ray.origin;
			vector3f dir = ray.end - ray.origin;
			_foundInter = isIntersectionTri(v0, v1, v2, point, dir);
			// If its found
			if (_foundInter)
			{
				// save the triangle to render in red afterwards
				_intersection = vector3f(_triangles[i]->a, _triangles[i]->b, _triangles[i]->c);
				// returns true
				return _foundInter;
			}
			// If its not found we set a default value of -1
			else _intersection = vector3f(-1, -1, -1);
				
		}
		return false;
	}
	return _foundInter;
}

// Function that checks intersection between ray and triangle
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

// Returns the intersected triangle
vector3f &Octree::getTriangInter()
{
	return _intersection;
}

void Octree::resetTChecked()
{
	_trianglesChecked = 0;
}

int Octree::getTChecked()
{
	return _trianglesChecked;
}

void Octree::resetNChecked()
{
	_nodesChecked = 0;
}

int Octree::getNChecked()
{
	return _nodesChecked;
}