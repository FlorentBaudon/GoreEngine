#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define PI 3.1415f

using namespace glm;

//Player
// float px = 288, py = 288;
// const float pdx = 1, pdy = 0;

vec2 world_forward = vec2(1,0);

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

void CreateGLWindow()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("G0r3ng1n3");
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	gluOrtho2D(0,800, 600,0);
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
	
	int rayLength = 1000;
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(p.x, p.y);
	glVertex2f(p.x + fwd.x * rayLength, p.y + fwd.y * rayLength);
	glEnd();
}

void drawRayCast(vec2 p, vec2 fwd)
{
	float dx = 0, dy = 0;
	
	if(abs(fwd.y) > abs(fwd.x))
	{
		
		 dy = (fwd.y > 0) ? ceil(p.y / 64)*64 - p.y : floor(p.y / 64)*64 - p.y;
		 dx =  tan(p_angle + PI/2) * dy;	
	}else
	{
		dx = (fwd.x > 0) ? ceil(p.x / 64)*64 - p.x : floor(p.x / 64)*64 - p.x;
		dy =  -tan(p_angle) * dx;
	}
	
	std::cout << dx + p.x << " - " << dy +  p.y << std::endl;
	glColor3f(0, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x + dx , p.y + dy);
	glEnd();
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	
	refreshPlayerDatas();
	drawPlayer(player_pos, player_forward);
	drawRayCast(player_pos, player_forward);

	glutSwapBuffers();
}

void processInput(unsigned char key, int x, int y) 
{
	if (key == 'z') { player_pos.y -= 5; }
	if (key == 's') { player_pos.y += 5; }
	if (key == 'q') { p_angle += radians(10.0f); }
	if (key == 'd') { p_angle -= radians(10.0f); }

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	CreateGLWindow();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(processInput);
	glutMainLoop();

	return 0;
}