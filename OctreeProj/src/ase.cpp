#include "ase.h"
#include "text.h"

#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

using namespace std;

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

	int i;
	for (i=0; i<m_vertices.size(); i++)
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

	for (i=0;i<m_triangles.size(); i++)
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
	int i;
	for (i=0;i<m_triangles.size();i++)
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
}

void CASEModel::loadMinMax()
{
	
	auto min_max_X = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.x < right.x; });
	auto min_max_Y = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.y < right.y; });
	auto min_max_Z = minmax_element(m_vertices.begin(), m_vertices.end(),
		[](vector3f left, vector3f right) {return left.z < right.z; });

	_maxBB.x = min_max_X.second->x;
	_maxBB.y = min_max_Y.second->y;
	_maxBB.z = min_max_Z.second->z;

	_minBB.x = min_max_X.first->x;
	_minBB.y = min_max_Y.first->y;
	_minBB.z = min_max_Z.first->z;

	cout << "MAXBB: " << _maxBB.x << ", " << _maxBB.y << ", " << _maxBB.z << endl;
	cout << "MINBB: " << _minBB.x << ", " << _minBB.y << ", " << _minBB.z << endl;
}

void CASEModel::renderBB()
{
	glBegin(GL_LINES);
	for (int i = 0; i<24; i++)
	{
		//FLOOR
		glVertex3f(_minBB.x, _minBB.y, _minBB.z);
		glVertex3f(_maxBB.x, _minBB.y, _minBB.z);

		glVertex3f(_maxBB.x, _minBB.y, _minBB.z);
		glVertex3f(_maxBB.x, _maxBB.y, _minBB.z);

		glVertex3f(_maxBB.x, _maxBB.y, _minBB.z);
		glVertex3f(_minBB.x, _maxBB.y, _minBB.z);

		glVertex3f(_minBB.x, _maxBB.y, _minBB.z);
		glVertex3f(_minBB.x, _minBB.y, _minBB.z);

		//COLUMNS
		glVertex3f(_minBB.x, _minBB.y, _minBB.z);
		glVertex3f(_minBB.x, _minBB.y, _maxBB.z);

		glVertex3f(_maxBB.x, _minBB.y, _minBB.z);
		glVertex3f(_maxBB.x, _minBB.y, _maxBB.z);

		glVertex3f(_maxBB.x, _maxBB.y, _minBB.z);
		glVertex3f(_maxBB.x, _maxBB.y, _maxBB.z);

		glVertex3f(_minBB.x, _maxBB.y, _minBB.z);
		glVertex3f(_minBB.x, _maxBB.y, _maxBB.z);

		//ROOF
		glVertex3f(_minBB.x, _minBB.y, _maxBB.z);
		glVertex3f(_maxBB.x, _minBB.y, _maxBB.z);

		glVertex3f(_maxBB.x, _minBB.y, _maxBB.z);
		glVertex3f(_maxBB.x, _maxBB.y, _maxBB.z);

		glVertex3f(_maxBB.x, _maxBB.y, _maxBB.z);
		glVertex3f(_minBB.x, _maxBB.y, _maxBB.z);

		glVertex3f(_minBB.x, _maxBB.y, _maxBB.z);
		glVertex3f(_minBB.x, _minBB.y, _maxBB.z);
	}
	glEnd();

}