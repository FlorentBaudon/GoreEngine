#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Raycaster.h"
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

Raycaster* raycaster;


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

int CreateGLWindow(GLFWwindow** windowRef, int posX, int posY, const char* title)
{
	*windowRef = glfwCreateWindow(resX, resY, title, NULL, NULL);

	if (*windowRef == NULL) {
		std::cout << "Failed to open  GLFW window.\n";
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(*windowRef, posX, posY);
	glfwMakeContextCurrent(*windowRef);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, resX, resY, 0.0f, 0.0f, 1.0f);

	return 1;
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
	raycaster->scanEnv(player->position, player->angle, fov);
}

void display2D() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	raycaster->testRaycasts(player->position, player->angle, fov);
	drawPlayer(player->position, player->forward);
}

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float s = 5.0f;
	vec2 d = vec2(0, 0);

	if (key == GLFW_KEY_Z) { d.x += s; }
	if (key == GLFW_KEY_S) { d.x -= s; }
	if (key == GLFW_KEY_A) { d.y -= s; }
	if (key == GLFW_KEY_D) { d.y += s; }
	if (key == GLFW_KEY_Q) { player->turn(  radians(10.0f) ); }
	if (key == GLFW_KEY_E) { player->turn( -radians(10.0f) ); }

	player->move(d);

}

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
		return -1;
	}


	raycaster = new Raycaster(mapS, map, mapX, mapY, fov, resX, resY);

	GLFWwindow* window3D = 0;
	GLFWwindow* window2D = 0;

	CreateGLWindow(&window3D, 1000, 50, "G0r3ng1n3 - 3D");
	if (bDebugViewer)
	{
		CreateGLWindow(&window2D, 0, 50, "G0r3ng1n3 - 2D Viewer");
	}


	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	glfwMakeContextCurrent(window3D);

	while (!glfwWindowShouldClose(window3D))
	{
		glfwMakeContextCurrent(window3D);
		glfwSetKeyCallback(window3D, processInput);
		display3D();
		glfwSwapBuffers(window3D);

		if (bDebugViewer)
		{
			glfwMakeContextCurrent(window2D);
			glfwSetKeyCallback(window2D, processInput);
			display2D();
			glfwSwapBuffers(window2D);
		}

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}