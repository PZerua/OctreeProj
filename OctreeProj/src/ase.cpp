#include "ase.h"

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

CASEModel::CASEModel()
{

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

void CASEModel::render() const
{	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glBegin(GL_TRIANGLES);
	for (unsigned i=0;i<m_triangles.size();i++)
	{
		const triangle & t = m_triangles[i];
		/*
		vector3 normal;
		normal = cross(
			m_vertices[t.b] - m_vertices[t.a],
			m_vertices[t.c] - m_vertices[t.a]);
		normal.normalize();

		
		glNormal3fv(normal);
		*/
		glVertex3fv(m_vertices[t.a]);
		glVertex3fv(m_vertices[t.b]);
		glVertex3fv(m_vertices[t.c]);
	}
	glEnd();

	_octree->renderBox();
}

vector3f *CASEModel::loadMinMax()
{
	static vector3f minmax[2];

	auto min_max_X = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.x < right.x; });
	auto min_max_Y = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.y < right.y; });
	auto min_max_Z = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.z < right.z; });

	minmax[0].x = min_max_X.first->x;
	minmax[0].y = min_max_Y.first->y;
	minmax[0].z = min_max_Z.first->z;

	minmax[1].x = min_max_X.second->x;
	minmax[1].y = min_max_Y.second->y;
	minmax[1].z = min_max_Z.second->z;

	return minmax;
}

void CASEModel::createOctree()
{
	vector3f *minmax = loadMinMax();
	vector3f hDimension((minmax[1].x - minmax[0].x) / 2, (minmax[1].y - minmax[0].y) / 2, (minmax[1].z - minmax[0].z) / 2);
	vector3f origin = (minmax[0] + minmax[1]) / 2.0f;

	_octree = new Octree(origin, hDimension, 0, 1000);
	_octree->createCBox(minmax[0], minmax[1]);
	_octree->insert(m_triangles);
	_octree->makeOctree(m_vertices);
}