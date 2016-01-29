#pragma once

#include <iostream>

// global includes
#include <GL/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

// local defines
#define LEFTMOUSE 0
#define MIDDLEMOUSE 1
#define RIGHTMOUSE 2

using namespace std;

class triangle
{
public:
	union
	{
		struct { unsigned int t[3]; };
		struct { unsigned int a, b, c; };
	};

};
