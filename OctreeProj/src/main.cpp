#include "header.h"
#include "ase.h"
#include "vector3f.h"
#include "matrix4x4f.h"

// global variables
vector3f g_vEye(0,1,-5);
vector3f g_vLook(0,0,1);
vector3f g_vRight(-1,0,0);
vector3f g_vUp(0,1,0);
int g_buttons[3];
int g_mouse_x,g_mouse_y;
int g_width, g_height;

// the mesh model
CASEModel g_model;
Octree *octr;

//2k8

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
	// prepare 2d viewport
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);

	drawString(690,10,"Press UP/DOWN/LEFT/RIGHT to navigate");
	drawString(690,40,"Press LEFT BUTTON to perform looking");
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glShadeModel(GL_FLAT);

	// setup camera
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(90,1.33,1,200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	matrix4x4f view;
	view.identity();

	g_vLook.normalize();

	g_vRight = crossProduct(g_vLook, g_vUp);
	g_vRight.normalize();

	g_vUp = crossProduct(g_vRight, g_vLook);
	g_vUp.normalize();

	view.m[0] =  g_vRight.x;
	view.m[1] =  g_vUp.x;
	view.m[2] = -g_vLook.x;
	view.m[3] =  0.0f;

	view.m[4] =  g_vRight.y;
	view.m[5] =  g_vUp.y;
	view.m[6] = -g_vLook.y;
	view.m[7] =  0.0f;

	view.m[8]  =  g_vRight.z;
	view.m[9]  =  g_vUp.z;
	view.m[10] = -g_vLook.z;
	view.m[11] =  0.0f;

	view.m[12] = -dotProduct(g_vRight, g_vEye);
	view.m[13] = -dotProduct(g_vUp, g_vEye);
	view.m[14] =  dotProduct(g_vLook, g_vEye);
	view.m[15] =  1.0f;


	glMultMatrixf( view.m );

	//render here

	// draw 3d model
	g_model.render();
	if (octr != NULL)
		octr->renderBox(vector3f(0.0, 1.0, 0.0));
	// ...
	//glutWireCube(5);
	// dibuixar un cub on cada banda té 1 de llarg
	//glDisable(GL_LIGHTING);
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
		case 13: break;
		/*
		//Teclas de 0 a 7
		case 48: break;
		case 49: break;
		case 50: break;
		case 51: break;
		case 52: break;
		case 53: break;
		case 54: break;
		case 55: break;*/
	}
	if (key >= 48 & key <= 55) {
		/*for
			if
				=true
			else
				=false*/
		cout << "KEY: " << (key - 48) << endl;
		octr = octr->getChild(key - 48);
		//cout << "Octree: " << oct. << endl;
	}
}

void parsekey_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_vEye += (g_vLook)*0.05f;
		break;
	case GLUT_KEY_DOWN:
		g_vEye -= (g_vLook)*0.05f;
		break;
	case GLUT_KEY_RIGHT:
		g_vEye += (g_vRight)*0.05f;
		break;
	case GLUT_KEY_LEFT:	
		g_vEye -= (g_vRight)*0.05f;
		break;
	}
}

void idle()
{
	display();
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
	glutInitWindowSize(640, 480);
	glutCreateWindow("Practice 1: Creating an Octree");
	glutDisplayFunc(display);
	glutKeyboardFunc(parsekey);
	glutSpecialFunc(parsekey_special);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// load a model
	//g_model.load("data\\teapot.ase");
	g_model.load("data/knot.ase");
	g_model.createOctree();
	octr = g_model.getOctree();
	//g_model.load("data\\terrain.ase");

	glutSwapBuffers();
	glutMainLoop();
	return 0;
}


#undef LEFTMOUSE
#undef MIDDLEMOUSE
#undef RIGHTMOUSE
