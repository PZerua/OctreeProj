#include "ase.h"

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

CASEModel::CASEModel()
{
	_octree = NULL;
}

bool CASEModel::load(const char* filename)
{
	CText t(filename);

	t.Seek("*MESH_NUMVERTEX");
	int n = t.GetInt();

	m_vertices.resize(n);

	t.Seek("*MESH_NUMFACES");
	n = t.GetInt();
	m_triangles.resize(n);

	for (unsigned i=0; i<m_vertices.size(); i++)
	{
		t.Seek("*MESH_VERTEX");
		if (t.GetInt()==i)
		{
			// MAX TO OPENGL CONVERSION: +x,+y,+z -> -x,+z,+y
			m_vertices[i].x=-t.GetFloat();
			m_vertices[i].z=t.GetFloat();
			m_vertices[i].y=t.GetFloat();

		}
		else
			return false;
	}

	for (unsigned i=0;i<m_triangles.size(); i++)
	{
		t.Seek("*MESH_FACE");
		if (t.GetInt()==i)
		{
			t.Seek("A:");
			m_triangles[i].a=t.GetInt();
			t.Seek("B:");
			m_triangles[i].b=t.GetInt();
			t.Seek("C:");
			m_triangles[i].c=t.GetInt();
		}
	}

	return true;
}

// Render the model or the triangles contained in each node
void CASEModel::render(const vector<triangle *> &triangles, const vector3f &inter) const
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	bool isIntersection = false;
	//cout << inter.x << " " << inter.y << " " << inter.z << endl;
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	for (unsigned i=0; i<triangles.size(); i++)
	{
		const triangle * t = triangles[i];

		if (inter.x == t->a && inter.y == t->b && inter.z == t->c)
			isIntersection = true;
		else isIntersection = false;

		// If its intersection, we render it in red
		if (isIntersection)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3fv(m_vertices[t->a]);
			glVertex3fv(m_vertices[t->b]);
			glVertex3fv(m_vertices[t->c]);
		}
		// Else, we render it in yellow
		else
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3fv(m_vertices[t->a]);
			glVertex3fv(m_vertices[t->b]);
			glVertex3fv(m_vertices[t->c]);
		}
	}
	glEnd();
}

// Loads the min and max vertices of the figure
vector3f *CASEModel::loadMinMax()
{
	auto min_max_X = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.x < right.x; });
	auto min_max_Y = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.y < right.y; });
	auto min_max_Z = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.z < right.z; });

	_minmax[0].x = min_max_X.first->x;
	_minmax[0].y = min_max_Y.first->y;
	_minmax[0].z = min_max_Z.first->z;

	_minmax[1].x = min_max_X.second->x;
	_minmax[1].y = min_max_Y.second->y;
	_minmax[1].z = min_max_Z.second->z;

	return _minmax;
}

// Creates and octree and sets the initial values
void CASEModel::createOctree()
{
	// We save pointers of triangles to be more efficient
	vector<triangle *> trianglesPointer;
	for (unsigned i = 0; i < m_triangles.size(); i++)
	{
		trianglesPointer.push_back(&m_triangles[i]);
	}
	vector3f *minmax = loadMinMax();
	vector3f hDimension((minmax[1].x - minmax[0].x) / 2, (minmax[1].y - minmax[0].y) / 2, (minmax[1].z - minmax[0].z) / 2);
	// The origin is in the middle of min max
	vector3f origin = (minmax[0] + minmax[1]) / 2.0f;

	// We create the octree
	_octree = new Octree(origin, hDimension, 0, 1000, NULL);
	_octree->createCBox(minmax[0], minmax[1]);
	_octree->insert(trianglesPointer);
	_octree->makeOctree(m_vertices);
}

Octree *CASEModel::getOctree() {
	return _octree;
}

vector3f *CASEModel::getMinMax()
{
	return _minmax;
}

vector<vector3f> &CASEModel::getVertices()
{
	return m_vertices;
}