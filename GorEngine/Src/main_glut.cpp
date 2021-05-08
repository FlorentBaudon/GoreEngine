#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "RaycastTools.h"
#include "GlewDrawFunctions.h"
#include "Player.h";

#define PI 3.1415f
#define DEG2RAD(x) x*(3.1415f/180.f)
#define RAD2DEG(x) x*(180.f/3.1415f)

using namespace glm;

bool once = true;

bool bDebugViewer = true;

int resX = 600, resY = 600;

int TwoDWindow, ThreeDWindow;

vec2 world_forward = vec2(1,0);
vec2 world_right = vec2(0, 1);
float fov = DEG2RAD(60.f);


/********* Player **********/
Player* player = new Player(vec2(288, 70), world_forward, world_right, radians(-90.f));

//Map
int mapX = 8, mapY = 8, mapS = 64, gridS = 1;
int map[] =
{
1,1,1,1,2,1,1,1,
1,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,2,2,2,
1,0,0,0,0,0,0,1,
1,1,1,3,0,2,2,2,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1
};

int Create2DGLWindow(int posX, int posY)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(resX, resY);
	int id = glutCreateWindow("G0r3ng1n3 - 2D Viewer");
	glutPositionWindow(posX, posY);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	gluOrtho2D(0,resX, resY,0);

	return id;
}

int Create3DGLWindow(int posX, int posY)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(resX, resY);
	int id = glutCreateWindow("G0r3ng1n3 - 3D");
	glutPositionWindow(posX, posY);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	gluOrtho2D(0, resX, resY, 0);

	return id;
}

void drawMap() 
{
	int xo = 0, yo = 0;

	for (int y = 0; y < mapY; y++) 
	{
		for (int x = 0; x < mapX; x++)
		{
			int c = map[x + (mapX * y)];

			switch (c)
			{
				case 1 :
					glColor3f(1, 1, 1);
					break;
				case 2:
					glColor3f(0, 1, 0);
					break;
				case 3:
					glColor3f(1, 0, 0);
					break;
				default:
					glColor3f(0, 0, 0);
					break;
			}

			xo = mapS * x;
			yo = mapS * y;

			glBegin(GL_QUADS);
			glVertex2i(xo + gridS, yo + gridS);
			glVertex2i(xo + gridS, yo + mapS - gridS);
			glVertex2i(xo + mapS - gridS, yo + mapS - gridS);
			glVertex2i(xo + mapS - gridS, yo + gridS);
			glEnd();
		}
	}

}

void refreshPlayerDatas()
{
	//mat4 rotMat = rotate(mat4(1.0f), player->angle, vec3(0.0f, 0.0f, 1.0f));

	//player->forward = glm::vec4(world_forward,0,0) * rotMat;
}

void drawPlayer(vec2 p, vec2 fwd)
{	
	glColor3f(1, 0, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();

	//rotate player dir vector
	
	 int rayLength = 20;
	 glLineWidth(4);
	 glBegin(GL_LINES);
	 glVertex2f(p.x, p.y);
	 glVertex2f(p.x + fwd.x * rayLength, p.y + fwd.y * rayLength);
	 glEnd();
}

void display3D() 
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		refreshPlayerDatas();
		scanEnv(player->position, player->angle, mapS, map, mapX, mapY, fov);

		glutSwapBuffers();
}

void display2D() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	testRaycasts(player->position, player->angle, mapS, map, mapX, mapY, fov);
	drawPlayer(player->position, player->forward);

	glutSwapBuffers();
}

void processInput(unsigned char key, int x, int y) 
{
	float s = 5.0f;
	vec2 d = vec2(0, 0);
	if (key == 'z') { d.x += s; }
	if (key == 's') { d.x -= s; }
	if (key == 'q') { d.y -= s; }
	if (key == 'd') { d.y += s; }
	if (key == 'a') { player->turn(  radians(10.0f) ); }
	if (key == 'e') { player->turn( -radians(10.0f) ); }

	player->move(d);

	glutPostRedisplay();
}

void idle() 
{
	glutSetWindow(ThreeDWindow);
	glutPostRedisplay();
	glutSetWindow(TwoDWindow);
	glutPostRedisplay();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	ThreeDWindow = Create3DGLWindow(1000, 0);
	glutKeyboardFunc(processInput);

	glutDisplayFunc(display3D);

	if (bDebugViewer) {

		TwoDWindow = Create2DGLWindow(0, 0);

		glutDisplayFunc(display2D);
		glutKeyboardFunc(processInput);
		glutIdleFunc(idle); //sync windows
	}
		
	glutMainLoop();

	return 0;
}