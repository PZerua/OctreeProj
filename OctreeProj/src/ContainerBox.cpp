#include "ContainerBox.h"


// Class used to draw boxes, in this case the Bounding Box and the box of each node of the octree
ContainerBox::ContainerBox(const vector3f &minCorner, const vector3f &maxCorner)
{
	_minCorner = minCorner;
	_maxCorner = maxCorner;

}

ContainerBox::~ContainerBox()
{

}

void ContainerBox::render(vector3f color)
{
	glBegin(GL_LINES);

	// Set Color
	glColor3f(color.x, color.y, color.z);
	
	// FLOOR
	glVertex3f(_minCorner.x, _minCorner.y, _minCorner.z);
	glVertex3f(_maxCorner.x, _minCorner.y, _minCorner.z);

	glVertex3f(_maxCorner.x, _minCorner.y, _minCorner.z);
	glVertex3f(_maxCorner.x, _maxCorner.y, _minCorner.z);

	glVertex3f(_maxCorner.x, _maxCorner.y, _minCorner.z);
	glVertex3f(_minCorner.x, _maxCorner.y, _minCorner.z);

	glVertex3f(_minCorner.x, _maxCorner.y, _minCorner.z);
	glVertex3f(_minCorner.x, _minCorner.y, _minCorner.z);

	// COLUMNS
	glVertex3f(_minCorner.x, _minCorner.y, _minCorner.z);
	glVertex3f(_minCorner.x, _minCorner.y, _maxCorner.z);

	glVertex3f(_maxCorner.x, _minCorner.y, _minCorner.z);
	glVertex3f(_maxCorner.x, _minCorner.y, _maxCorner.z);

	glVertex3f(_maxCorner.x, _maxCorner.y, _minCorner.z);
	glVertex3f(_maxCorner.x, _maxCorner.y, _maxCorner.z);

	glVertex3f(_minCorner.x, _maxCorner.y, _minCorner.z);
	glVertex3f(_minCorner.x, _maxCorner.y, _maxCorner.z);

	// ROOF
	glVertex3f(_minCorner.x, _minCorner.y, _maxCorner.z);
	glVertex3f(_maxCorner.x, _minCorner.y, _maxCorner.z);

	glVertex3f(_maxCorner.x, _minCorner.y, _maxCorner.z);
	glVertex3f(_maxCorner.x, _maxCorner.y, _maxCorner.z);

	glVertex3f(_maxCorner.x, _maxCorner.y, _maxCorner.z);
	glVertex3f(_minCorner.x, _maxCorner.y, _maxCorner.z);

	glVertex3f(_minCorner.x, _maxCorner.y, _maxCorner.z);
	glVertex3f(_minCorner.x, _minCorner.y, _maxCorner.z);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glEnd();
}

// We need the min and max corner of the box
void ContainerBox::setMinMax(const vector3f &minCorner, const vector3f &maxCorner)
{
	_minCorner = minCorner;
	_maxCorner = maxCorner;
}

vector3f ContainerBox::getMax()
{
	return _maxCorner;
}

vector3f ContainerBox::getMin()
{
	return _minCorner;
}