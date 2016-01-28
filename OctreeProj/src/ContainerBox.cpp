#include "ContainerBox.h"

ContainerBox::ContainerBox(const vector3f &minCorner, const vector3f &maxCorner)
{
	_minCorner = minCorner;
	_maxCorner = maxCorner;

}

ContainerBox::~ContainerBox()
{

}

void ContainerBox::render()
{
	glBegin(GL_LINES);

	// Set Color
	glColor3f(0.0f, 1.0f, 0.0f);

	//cout << "MAX: " << _maxCorner.x << " " << _maxCorner.y << " " << _maxCorner.z << endl;
	//cout << "MIN: " << _minCorner.x << " " << _minCorner.y << " " << _minCorner.z << endl;

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