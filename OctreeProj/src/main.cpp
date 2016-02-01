#include "header.h"
#include "ase.h"
#include "vector3f.h"
#include "matrix4x4f.h"
#include "ray.h"

// global variables
vector3f g_vEye(0,1,-5);
vector3f g_vLook(0,0,1);
vector3f g_vRight(-1,0,0);
vector3f g_vUp(0,1,0);
int g_buttons[3];
int g_mouse_x,g_mouse_y;
int rClickX = 0, rClickY = 0;
int g_width, g_height;

// bools used to cast movement
bool MOVE_UP = false;
bool MOVE_DOWN = false;
bool MOVE_RIGHT = false;
bool MOVE_LEFT = false;
bool renderRay = false;

// the mesh model
CASEModel g_model;
// the octree
Octree *octr;
// the ray
Ray ray;

void drawString(int x, int y, const char* string)
{
	int i, len;
	glRasterPos2f(x, y);
	glColor3f(1.0f, 1.0f, 1.0f);
	len = strlen (string);
	for (i=0;i<len;i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
	}
}

void help()
{
	glDisable(GL_LIGHTING);
	// prepare viewport
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);

	string xpos = to_string(octr->getTriangInter().x);
	string ypos = to_string(octr->getTriangInter().y);
	string zpos = to_string(octr->getTriangInter().z);
	string tPicked = "Triangle picked: " + xpos + " " + ypos + " " + zpos;
	string tChecked = "Number of triangles checked: " + to_string(octr->getTChecked());
	string vChecked = "Number of nodes checked: " + to_string(octr->getNChecked());


	drawString(690,10,"Press UP/DOWN/LEFT/RIGHT to navigate");
	drawString(690,40,"Press LEFT BUTTON to perform looking");
	drawString(690, 70, "Press RIGHT BUTTON to throw ray");
	drawString(10, 70, vChecked.c_str());
	drawString(10, 40, tChecked.c_str());
	drawString(10, 10, tPicked.c_str());

	octr->resetTChecked();
	octr->resetNChecked();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// setup camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, SCREEN_WIDTH/SCREEN_HEIGHT, 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	matrix4x4f view;
	view.identity();

	g_vLook.normalize();

	g_vRight = crossProduct(g_vLook, g_vUp);
	g_vRight.normalize();

	g_vUp = crossProduct(g_vRight, g_vLook);
	g_vUp.normalize();

	view.m[0] = g_vRight.x;
	view.m[1] = g_vUp.x;
	view.m[2] = -g_vLook.x;
	view.m[3] = 0.0f;

	view.m[4] = g_vRight.y;
	view.m[5] = g_vUp.y;
	view.m[6] = -g_vLook.y;
	view.m[7] = 0.0f;

	view.m[8] = g_vRight.z;
	view.m[9] = g_vUp.z;
	view.m[10] = -g_vLook.z;
	view.m[11] = 0.0f;

	view.m[12] = -dotProduct(g_vRight, g_vEye);
	view.m[13] = -dotProduct(g_vUp, g_vEye);
	view.m[14] = dotProduct(g_vLook, g_vEye);
	view.m[15] = 1.0f;


	glMultMatrixf(view.m);

	//render here

	// draw 3d model
	if (octr != NULL)
	{
		g_model.render(octr->getTriangles(), octr->getTriangInter());
		octr->renderBox(vector3f(0.0, 0.0, 1.0));
	}

	if (renderRay)
	{

		double matModelView[16], matProjection[16];
		int viewport[4];

		// get matrix and viewport:
		glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
		glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		// window pos of mouse, Y is inverted on Windows
		double winX = (double)rClickX;
		double winY = viewport[3] - (double)rClickY;

		GLdouble sX, sY, sZ;

		// get point on the 'near' plane (third param is set to 0.0)
		gluUnProject(winX, winY, 0.0, matModelView, matProjection,
			viewport, &sX, &sY, &sZ);

		GLdouble gX, gY, gZ;

		// get point on the 'far' plane (third param is set to 1.0)
		gluUnProject(winX, winY, 1.0, matModelView, matProjection,
			viewport, &gX, &gY, &gZ);

		ray.origin = vector3f(sX, sY, sZ);
		ray.end = vector3f(gX, gY, gZ);

		octr->isIntersection(ray, g_model.getVertices());

	}

	help();

	glFlush();
	glutSwapBuffers();

}

void reshape(int w, int h)
{
	g_width = w;
	g_height = h;
	glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, w, h);
}

void parsekey(unsigned char key, int x, int y)
{
	switch (key)
	{
		//Tecla ESC
		case 27: exit(0); break;
		//Tecla Enter
		case 13: 
			while(octr->getPointerToFather() != NULL)
				octr = octr->getPointerToFather();
			break;
		case 8:
			if (octr->getPointerToFather() != NULL)
				octr = octr->getPointerToFather();
			break;
	}
	if (key >= 48 && key <= 55) {
		if (octr->getChild(key - 48) != NULL)
			octr = octr->getChild(key - 48);
	}
}

void parsekey_specialDOWN(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		MOVE_UP = true;
		break;
	case GLUT_KEY_DOWN:
		MOVE_DOWN = true;
		break;
	case GLUT_KEY_RIGHT:
		MOVE_RIGHT = true;
		break;
	case GLUT_KEY_LEFT:	
		MOVE_LEFT = true;
		break;
	}
}

void parsekey_specialUP(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		MOVE_UP = false;
		break;
	case GLUT_KEY_DOWN:
		MOVE_DOWN = false;
		break;
	case GLUT_KEY_RIGHT:
		MOVE_RIGHT = false;
		break;
	case GLUT_KEY_LEFT:
		MOVE_LEFT = false;
		break;
	}
}

void idle()
{
	display();
}

// Sets the camera position (checked every 16 ms => 1/60Hz)
void moveCamera(int value)
{
	if (MOVE_UP)
		g_vEye += (g_vLook)*0.05f;
	if (MOVE_DOWN)
		g_vEye -= (g_vLook)*0.05f;
	if (MOVE_RIGHT)
		g_vEye += (g_vRight)*0.05f;
	if (MOVE_LEFT)
		g_vEye -= (g_vRight)*0.05f;
	glutTimerFunc(16, moveCamera, 0);
}


void motion(int x, int y)
{
	int dx = x - g_mouse_x;
	int dy = y - g_mouse_y;

	if (g_buttons[LEFTMOUSE] == 1)
	{
		matrix4x4f matRotation;
		if( dy != 0 )
		{
			matRotation.rotate( -(float)dy / 3.0f, g_vRight );
			matRotation.transformVector( &g_vLook );
			matRotation.transformVector( &g_vUp );
		}

		if( dx != 0 )
		{
			matRotation.rotate( -(float)dx / 3.0f, vector3f(0.0f, 1.0f, 0.0f) );
			matRotation.transformVector( &g_vLook );
			matRotation.transformVector( &g_vUp );
		}

	}
	else if (g_buttons[RIGHTMOUSE] == 1)
	{
		renderRay = true;
		rClickX = x;
		rClickY = y;
	}

	g_mouse_x = x;
	g_mouse_y = y;

}

void mouse(int button, int state, int x, int y)
{
    g_mouse_x = x;
    g_mouse_y = y;
    switch (button) {
    case GLUT_LEFT_BUTTON:
        g_buttons[LEFTMOUSE] = (state == GLUT_DOWN);
        break;
    case GLUT_MIDDLE_BUTTON:
        g_buttons[MIDDLEMOUSE] = (state == GLUT_DOWN);
        break;
    case GLUT_RIGHT_BUTTON:
        g_buttons[RIGHTMOUSE] = (state == GLUT_DOWN);
        break;
    }
}

int main(int arg, char** argv)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Practice 1: Creating an Octree");
	glutDisplayFunc(display);
	glutKeyboardFunc(parsekey);
	glutSpecialFunc(parsekey_specialDOWN);
	glutSpecialUpFunc(parsekey_specialUP);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(16, moveCamera, 0);

	// load a model
	g_model.load("data/knot.ase");
	g_model.createOctree();
	octr = g_model.getOctree();

	glutSwapBuffers();
	glutMainLoop();
	return 0;
}


#undef LEFTMOUSE
#undef MIDDLEMOUSE
#undef RIGHTMOUSE
