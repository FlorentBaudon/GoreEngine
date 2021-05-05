#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RaycastTools.h"
#include "GlewDrawFunctions.h"

#define PI 3.1415f
#define DEG2RAD(x) x*(3.1415f/180.f)
#define RAD2DEG(x) x*(180.f/3.1415f)

using namespace glm;

bool checkIfIntersect(vec2 p, int cellSize, int map[], int mapSizeX, int mapSizeY)
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

vec2 findHorizontalIntersect(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY) 
{
	vec2 p = vec2(0, 0);
	vec2 delta = vec2(0, 0);

	bool bIntersect = false;

	if (sin(angle) > 0.001f) 
	{
		p.y = int(pos.y / cellSize) * cellSize - pos.y - 0.0001f;
		p.x = tan(angle + PI / 2) * p.y;
		p = pos + p;

		delta.x = cellSize / tan(angle);
		delta.y = -cellSize;
	}
	else if(sin(angle) < -0.001f)
	{
		p.y = int(pos.y / cellSize) * cellSize - pos.y + cellSize;
		p.x = tan(angle + PI / 2) * p.y;
		p = pos + p;

		delta.x = cellSize / tan(-angle);
		delta.y = +cellSize;
	}
	else {
		return vec2(100000, 100000);
	}
	
	
	bIntersect = checkIfIntersect(p, cellSize, map, mapSizeX, mapSizeY);


	while (!bIntersect) 
	{
		p = p + delta;
		bIntersect = checkIfIntersect(p, cellSize, map, mapSizeX, mapSizeY);
	}

	return p;

}

vec2 findVerticalIntersect(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY) 
{

	vec2 p = vec2(0, 0);
	vec2 d = vec2(0, 0);

	bool bIntersect = false;

	if (cos(angle) < -0.001f) 
	{
		p.x = int(pos.x / cellSize) * cellSize - pos.x - 0.0001f;
		p.y = -tan(angle) * p.x;
		p = pos + p;

		d.x = -cellSize;
		d.y = cellSize * tan(angle);

	}
	else if (cos(angle) > 0.001f)
	{
		p.x = int(pos.x / cellSize) * 64 - pos.x + cellSize;
		p.y = -tan(angle) * p.x;
		p = pos + p;

		d.x = cellSize;
		d.y = cellSize * tan(-angle);
	}
	else 
	{
		return vec2(100000, 100000); // return infinite point 
	}

	bIntersect = checkIfIntersect(p, cellSize, map, mapSizeX, mapSizeY);
	
	while (!bIntersect)
	{
		p = p + d;
		bIntersect = checkIfIntersect(p, cellSize, map, mapSizeX, mapSizeY);
	}

	return p;
}

void drawRaycast(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY)
{
	vec2 pH = findHorizontalIntersect(pos, angle, cellSize, map, mapSizeX, mapSizeY);
	vec2 pV = findVerticalIntersect(pos, angle, cellSize, map, mapSizeX, mapSizeY);

	float dH =length(pH-pos);
	float dV =length(pV-pos);

	drawLine(pos, (dH < dV) ? pH : pV, vec3(0,1,0));
}

void scanEnv(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, float fov)
{
	fov = DEG2RAD(90);
	float r_angle = angle - fov/2;
	float step = 0.1f;

	for (float i = 0; i < RAD2DEG(fov); i+=step)
	{
		drawRaycast(pos, r_angle, cellSize, map, mapSizeX, mapSizeY);

		r_angle += DEG2RAD(step);

		if (r_angle < 0) r_angle += 2 * PI;
		if (r_angle > 2 * PI) r_angle -= 2 * PI;
	}
}




