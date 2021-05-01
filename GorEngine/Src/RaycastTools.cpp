#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RaycastTools.h"

#define PI 3.1415f

using namespace glm;

void drawLine(vec2 start, vec2 end)
{
	glColor3f(0, 0, 1);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();
}

void drawPoint(vec2 p, vec3 color = vec3(0,1,0))
{
	glColor3f(color.r, color.g, color.b);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}

vec2 findFirstPoint(vec2 p, vec2 fwd, float angle)
{

	vec2 d = vec2(0, 0);

	if (abs(fwd.y) > abs(fwd.x))
	{

		d.y = (fwd.y > 0) ? ceil(p.y / 64) * 64 - p.y : floor(p.y / 64) * 64 - p.y;
		d.x = tan(angle + PI / 2) * d.y;
	}
	else
	{
		//d.x = (fwd.x > 0) ? ceil(p.x / 64) * 64 - p.x : floor(p.x / 64) * 64 - p.x;
		//d.y = -tan(angle) * d.x;
	}

	return (p + d);
}

void findHorizontalIntersect(vec2 pos, vec2 fwd, float angle, int cellSize) //, int map[], int mapSizeX, int mapSizeY) 
{
	
	vec2 p = vec2(0, 0);
	vec2 d = vec2(0, 0);

	p.y = (fwd.y > 0) ? ceil(pos.y / 64) * 64 - pos.y : floor(pos.y / 64) * 64 - pos.y;
	p.x = tan(angle + PI / 2) * p.y;
	p = pos + p;

	drawPoint(p);

	//calculate dx and dy
	d.x = cellSize/tan((fwd.y < 0) ? +angle : -angle);
	d.y = (fwd.y > 0) ? +cellSize : -cellSize;
	int bIntersect = 0;
	while (bIntersect < 2) 
	{
		p = p + d;
		drawPoint(p);
		bIntersect++;
	}

}

void findVerticalIntersect(vec2 pos, vec2 fwd, float angle, int cellSize) //, int map[], int mapSizeX, int mapSizeY) 
{

	vec2 p = vec2(0, 0);
	vec2 d = vec2(0, 0);

	p.x = (fwd.x > 0) ? ceil(pos.x / 64) * 64 - pos.x : floor(pos.x / 64) * 64 - pos.x;
	p.y = -tan(angle) * p.x;
	p = pos + p;

	drawPoint(p, vec3(0,1,1));

	//calculate dx and dy
	d.x = (fwd.x > 0) ? +cellSize : -cellSize;
	d.y = cellSize * tan((fwd.x < 0) ? +angle : -angle);
	
	int bIntersect = 0;
	while (bIntersect < 2)
	{
		p = p + d;
		drawPoint(p, vec3(0, 1, 1));
		bIntersect++;
	}

}

