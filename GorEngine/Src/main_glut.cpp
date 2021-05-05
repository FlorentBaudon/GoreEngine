#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "RaycastTools.h"

#define PI 3.1415f
#define DEG2RAD(x) x*(3.1415f/180.f)
#define RAD2DEG(x) x*(180.f/3.1415f)

using namespace glm;

int resX = 600, resY = 600;

vec2 world_forward = vec2(1,0);
float fov = DEG2RAD(180.f);

vec2 player_pos = vec2(288, 288);
vec2 player_forward = world_forward;

float p_angle = radians(60.0f);

//Map
int mapX = 8, mapY = 8, mapS = 64, gridS = 1;
int map[] =
{
1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,1,1,1,
1,0,0,0,0,0,0,1,
1,1,1,1,0,1,1,1,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1
};

void Create2DGLWindow(int posX, int posY)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(resX, resY);
	glutCreateWindow("G0r3ng1n3 - 2D Viewer");
	glutPositionWindow(posX, posY);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	gluOrtho2D(0,resX, resY,0);
}

void Create3DGLWindow(int posX, int posY)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(resX, resY);
	glutCreateWindow("G0r3ng1n3 - 3D");
	glutPositionWindow(posX, posY);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void drawMap() 
{
	int xo = 0, yo = 0;

	for (int y = 0; y < mapY; y++) 
	{
		for (int x = 0; x < mapX; x++)
		{
			int c = map[x + (mapX * y)];
			if (c == 1) 
			{
				glColor3f(1, 1, 1);
			}
			else 
			{
				glColor3f(0, 0, 0);
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
	mat4 rotMat = rotate(mat4(1.0f), p_angle, vec3(0.0f, 0.0f, 1.0f));

	player_forward = glm::vec4(world_forward,0,0) * rotMat;
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

void drawRayCast(vec2 p, vec2 fwd)
{
	scanEnv(p, p_angle, mapS, map, mapX, mapY, fov);
}

void display3D() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	refreshPlayerDatas();

	glutSwapBuffers();
}

void display2D() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	drawRayCast(player_pos, player_forward);
	drawPlayer(player_pos, player_forward);

	glutSwapBuffers();
}

void processInput(unsigned char key, int x, int y) 
{
	float s = 5.0f;
	if (key == 'z') { player_pos += (player_forward * s); }
	if (key == 's') { player_pos -= (player_forward * s); }
	if (key == 'q') { player_pos.x -= s; }
	if (key == 'd') { player_pos.x += s; }
	if (key == 'a') { p_angle += radians(10.0f); }
	if (key == 'e') { p_angle -= radians(10.0f); }

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	Create3DGLWindow(1000, 0);
	glutKeyboardFunc(processInput);
	glutDisplayFunc(display3D);

	Create2DGLWindow(0,0);
	
	glutDisplayFunc(display2D);

	glutMainLoop();

	return 0;
}