#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RaycastTools.h"

#define PI 3.1415f
#define DEG2RAD(x) x*(3.1415f/180.f)
#define RAD2DEG(x) x*(180.f/3.1415f)

using namespace glm;

void drawLine(vec2 start, vec2 end, vec3 color = vec3(0, 1, 0))
{
	glColor3f(color.r, color.g, color.b);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();
}

void drawPoint(vec2 p, vec3 color = vec3(0,1,1))
{
	glColor3f(color.r, color.g, color.b);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}

bool checkIfIntersect(vec2 p, vec2 dir, int cellSize, int map[], int mapSizeX, int mapSizeY)
{
	if (p.x > mapSizeX * cellSize || p.y > mapSizeY * cellSize || p.x < 0 || p.y < 0)
	{
		return true;
	}

	vec2 t_p = vec2(0, 0); //truncate position
	t_p.x = floor(p.x / cellSize);
	t_p.y = floor(p.y / cellSize);

	int tabPos = t_p.x + (mapSizeX * t_p.y);

	int cell = map[tabPos];

	if (cell > 0)
	{
		return true;
	}

	return false;
}

vec2 findHorizontalIntersect(vec2 pos, vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY) 
{
	vec2 p = vec2(0, 0);
	vec2 d = vec2(0, 0);

	bool bIntersect = false;
	if (dir.y < 0) 
	{
		p.y = int(pos.y / cellSize) * cellSize - pos.y - 0.0001f;
	}else 
	{
		p.y = int(pos.y / cellSize) * cellSize - pos.y + cellSize;
	}
	//p.y = (dir.y > 0) ? ceil(pos.y / cellSize) * cellSize - pos.y : floor(pos.y / cellSize) * cellSize - pos.y - 0.0001f;
	p.x = tan(angle + PI / 2) * p.y;
	p = pos + p;
	
	bIntersect = checkIfIntersect(p, dir, cellSize, map, mapSizeX, mapSizeY);

	//calculate dx and dy
	d.x = cellSize/tan((dir.y < 0.f) ? +angle : -angle);
	d.y = (dir.y > 0.f) ? +cellSize : -cellSize;
	
	while (!bIntersect) 
	{
		p = p + d;
		bIntersect = checkIfIntersect(p, dir, cellSize, map, mapSizeX, mapSizeY);
	}

	return p;

}

vec2 findVerticalIntersect(vec2 pos, vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY) 
{

	vec2 p = vec2(0, 0);
	vec2 d = vec2(0, 0);

	bool bIntersect = false;

	if (dir.x < 0) 
	{
		p.x = int(pos.x / cellSize) * cellSize - pos.x - 0.0001f;
	}else 
	{
		p.x = int(pos.x / cellSize) * 64 - pos.x + cellSize;
	}

	//p.x = (dir.x > 0) ? ceil(pos.x / cellSize) * 64 - pos.x : floor(pos.x / cellSize) * cellSize - pos.x - 0.0001f;
	p.y = -tan(angle) * p.x;
	p = pos + p;

	bIntersect = checkIfIntersect(p, dir, cellSize, map, mapSizeX, mapSizeY);

	//calculate dx and dy
	d.x = (dir.x > 0.f) ? +cellSize : -cellSize;
	d.y = cellSize * tan((dir.x < 0.f) ? +angle : -angle);
	
	while (!bIntersect)
	{
		p = p + d;
		bIntersect = checkIfIntersect(p, dir, cellSize, map, mapSizeX, mapSizeY);
	}

	return p;
}

void drawRaycast(vec2 pos, vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY)
{
	vec2 pH = findHorizontalIntersect(pos, dir, angle, cellSize, map, mapSizeX, mapSizeY);
	vec2 pV = findVerticalIntersect(pos, dir, angle, cellSize, map, mapSizeX, mapSizeY);

	float dH =length(pH-pos);
	float dV =length(pV-pos);
	
	drawPoint( (dH < dV) ? pH : pV, vec3(0,1,0));
	drawLine(pos, (dH < dV) ? pH : pV, vec3(0,1,0));
}

void scanEnv(vec2 pos, vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, float fov)
{
	fov = DEG2RAD(60);
	float r_angle = angle - (30.0f * 0.0174533f);

	if (r_angle < 0) r_angle += 2 * PI;
	if (r_angle > 2 * PI) r_angle -= 2 * PI;

	vec2 r_dir = vec2(cos(r_angle) * dir.x - sin(r_angle) * dir.y, sin(r_angle) * dir.x + cos(r_angle) * dir.y);

	for (int i = 0; i < RAD2DEG(fov); i++)
	{
		drawRaycast(pos, r_dir, r_angle, cellSize, map, mapSizeX, mapSizeY);

		r_angle += 0.0174533f;

		if (r_angle < 0) r_angle += 2 * PI;
		if (r_angle > 2 * PI) r_angle -= 2 * PI;
	}
	// 5.968834f
}




