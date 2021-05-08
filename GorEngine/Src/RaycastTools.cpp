#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RaycastTools.h"
#include "GlewDrawFunctions.h"

#define PI 3.1415f
#define DEG2RAD(x) ( (x)*( 3.1415f/ 180.f ) )
#define RAD2DEG(x) ( (x)*( 180.f/ 3.1415f ) )

using namespace glm;

int checkCellValue(vec2 p, int cellSize, int map[], int mapSizeX, int mapSizeY)
{
	if (p.x > mapSizeX * cellSize || p.y > mapSizeY * cellSize || p.x < 0 || p.y < 0)
	{
		return -1;
	}

	vec2 t_p = vec2(0, 0); //truncate position
	t_p.x = floor(p.x / cellSize);
	t_p.y = floor(p.y / cellSize);

	int tabPos = t_p.x + (mapSizeX * t_p.y);

	int cell = map[tabPos];

	return cell;
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
	
	
	bIntersect = (checkCellValue(p, cellSize, map, mapSizeX, mapSizeY) != 0);

	while (!bIntersect) 
	{
		p = p + delta;
		bIntersect = (checkCellValue(p, cellSize, map, mapSizeX, mapSizeY) != 0);
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

	bIntersect = (checkCellValue(p, cellSize, map, mapSizeX, mapSizeY) != 0);
	
	while (!bIntersect)
	{
		p = p + d;
		bIntersect = (checkCellValue(p, cellSize, map, mapSizeX, mapSizeY) != 0);
	}

	return p;
}

vec2 launchRaycast(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, bool bDebug=false)
{
	vec2 pH = findHorizontalIntersect(pos, angle, cellSize, map, mapSizeX, mapSizeY);
	vec2 pV = findVerticalIntersect(pos, angle, cellSize, map, mapSizeX, mapSizeY);

	float dH =length(pH-pos);
	float dV =length(pV-pos);

	if(bDebug)
		drawLine(pos, (dH < dV) ? pH : pV, vec3(0,1,0));

	return (dH < dV) ? pH : pV;
}



void testRaycasts(vec2 pos, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY, float fov, float precision)
{
	float r_angle = angle - fov/2;

	for (float i = 0; i < RAD2DEG(fov); i+=1)
	{
		launchRaycast(pos, r_angle, cellSize, map, mapSizeX, mapSizeY, true);

		r_angle += DEG2RAD(precision);

		if (r_angle < 0) r_angle += 2 * PI;
		if (r_angle > 2 * PI) r_angle -= 2 * PI;
	}
}

void scanEnvDeg(const vec2 pos, const float angle, const int cellSize, int map[], const int mapSizeX, const int mapSizeY, const float fov)
{
	float r_angle = angle + fov / 2;

	int resX = 600, resY = 600;
	const float dScreen = abs((resX / 2) / tan(fov / 2));

	int hMur = 64;

	for (int i = 0; i < RAD2DEG(fov); i+=1)

	{
		float a = atan( ((resX / 2)-i) / dScreen );

		vec2 p = launchRaycast(pos, r_angle, cellSize, map, mapSizeX, mapSizeY);
		float d = length(p - pos);
		int r = checkCellValue(p, cellSize, map, mapSizeX, mapSizeY);


		if (true)
		{
			float hp = dScreen * (hMur / d);

			vec2 start( (resX / RAD2DEG(fov)) * i, resY/2 - (hp / 2));
			vec2 end((resX / RAD2DEG(fov) * i), resY/2 + (hp / 2));

			vec3 color(0, 0, 0);

			switch (r)
			{
			case -1 :
				color = vec3(1, 1, 1);
				break;
			case 0:
				color = vec3(0, 0, 1);
				break;
			case 1:
				color = vec3(1, 0, 0);
				break;
			case 2:
				color = vec3(0, 1, 0);
				break;
			default:
				break;
			}

			drawLine(start, end, color, 5);
		}

		r_angle -= DEG2RAD(1);

		if (r_angle < 0) r_angle += 2 * PI;
		if (r_angle > 2 * PI) r_angle -= 2 * PI;

	}
}

void scanEnv(const vec2 pos, const float angle, const int cellSize, int map[], const int mapSizeX, const int mapSizeY, const float fov)
{
	const float r_angle = angle + fov / 2;

	int resX = 600, resY = 600;
	const float dScreen = (resX / 2) / tan(fov / 2);
	int hMur = 64;
	float angleStep = fov / 600;

	for (int i = 0; i <resX; i++)
	{
		float a = r_angle - angleStep * i;


		/****** Launch raycast, je n'appelle pas la fonction car j'ai besoin des infos de distance pour le shading (savoir si c'est un intersect horizontale ou verticale ****/
		vec2 pH = findHorizontalIntersect(pos, a, cellSize, map, mapSizeX, mapSizeY);
		vec2 pV = findVerticalIntersect(pos, a, cellSize, map, mapSizeX, mapSizeY);

		float dH = length(pH - pos);
		float dV = length(pV - pos);


		vec2 p = (dH < dV) ? pH : pV;
		/********************************************************************************************************/

		float d = length(p - pos);
		d *= cos(a - angle);// * cos (player_angle - a) to correct distance to avoid fishey effect
		int r = checkCellValue(p, cellSize, map, mapSizeX, mapSizeY);


		if (true)
		{
			float hp = dScreen * (hMur / d);

			vec2 start( i, resY/2 - (hp / 2));
			vec2 end(i, resY/2 + (hp / 2));

			vec3 color(0, 0, 0);

			switch (r)
			{
			case -1:
				color = vec3(1, 1, 1);
				break;
			case 0:
				color = vec3(0, 0, 1);
				break;
			case 1:
				color = vec3(1, 1, 1);
				break;
			case 2:
				color = vec3(0, 1, 0);
				break;
			case 3:
				color = vec3(1, 0, 0);
				break;
			default:
				break;
			}

			//""Lighting""
			if (dV < dH) 
			{
				color *= 0.7;
			}

			drawLine(start, end, color, 1);
		}


	}
}





