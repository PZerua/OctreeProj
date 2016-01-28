#ifndef _ASE_LOADER_
#define _ASE_LOADER_

#include "header.h"
#include <iostream>
#include "vector3f.h"
#include "text.h"
#include "Octree.h"

#ifdef _WIN32
#include <windows.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
class triangle
{
public:
	union
	{
		struct{unsigned int t[3];};
		struct{unsigned int a,b,c;};
	};
	
};

/////////////////////////////////////////////////////////////////////////////////////////
// Simple ASE File format Loader
/////////////////////////////////////////////////////////////////////////////////////////
class CASEModel
{
public:
	CASEModel();
	vector3f *loadMinMax();
	void createOctree();
	// primary load model method
	bool load(const char* filename);

	// gl render
	void render() const;

protected:
	std::vector<vector3f> m_vertices;
	std::vector<triangle> m_triangles;
	Octree *_octree;

};


#endif
