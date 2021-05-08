#include <glm/glm.hpp>

#include "Raycaster.h"
#include "GlewDrawFunctions.h"

#define PI 3.1415f
#define DEG2RAD(x) ( (x)*( 3.1415f/ 180.f ) )
#define RAD2DEG(x) ( (x)*( 180.f/ 3.1415f ) )

using namespace glm;

Raycaster::Raycaster(int cellSize, int* map, int mapSizeX, int mapSizeY, float fov, int xResolution, int yResolution) 
{
	this->cellSize = cellSize; 
	this->map = map; 
	this->mapSizeX = mapSizeX; 
	this->mapSizeY = mapSizeY; 
	this->xResolution = xResolution;
	this->yResolution = yResolution;
}

int Raycaster::checkCellValue(vec2 p)
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

vec2 Raycaster::findHorizontalIntersect(vec2 pos, float angle)
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
	else if (sin(angle) < -0.001f)
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


	bIntersect = (checkCellValue(p) != 0);

	while (!bIntersect)
	{
		p = p + delta;
		bIntersect = (checkCellValue(p) != 0);
	}

	return p;

}

vec2 Raycaster::findVerticalIntersect(vec2 pos, float angle)
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

	bIntersect = (checkCellValue(p) != 0);

	while (!bIntersect)
	{
		p = p + d;
		bIntersect = (checkCellValue(p) != 0);
	}

	return p;
}


vec2 Raycaster::drawRaycast(vec2 pos, float angle, bool bDebug)
{
	vec2 pH = this->findHorizontalIntersect(pos, angle);
	vec2 pV = this->findVerticalIntersect(pos, angle);

	float dH = length(pH - pos);
	float dV = length(pV - pos);

	if (bDebug)
		drawLine(pos, (dH < dV) ? pH : pV, vec3(0, 1, 0));

	return (dH < dV) ? pH : pV;
}



void Raycaster::testRaycasts(vec2 pos, float angle, float fov, float precision)
{
	float r_angle = angle - fov / 2;

	for (float i = 0; i < RAD2DEG(fov); i += 1)
	{
		this->drawRaycast(pos, r_angle, true);

		r_angle += DEG2RAD(precision);

		if (r_angle < 0) r_angle += 2 * PI;
		if (r_angle > 2 * PI) r_angle -= 2 * PI;
	}
}

void Raycaster::scanEnv(const vec2 pos, const float angle, const float fov)
{
	const float r_angle = angle + fov / 2;

	int resX = 600, resY = 600;
	const float dScreen = (resX / 2) / tan(fov / 2);
	int hMur = 64;
	float angleStep = fov / resX;

	for (int i = 0; i < resX; i++)
	{
		float a = r_angle - angleStep * i;


		/****** Launch raycast, je n'appelle pas la fonction car j'ai besoin des infos de distance pour le shading (savoir si c'est un intersect horizontale ou verticale ****/
		vec2 pH = this->findHorizontalIntersect(pos, a);
		vec2 pV = this->findVerticalIntersect(pos, a);

		float dH = length(pH - pos);
		float dV = length(pV - pos);


		vec2 p = (dH < dV) ? pH : pV;
		/********************************************************************************************************/

		float d = length(p - pos);
		d *= cos(a - angle);// * cos (player_angle - a) to correct distance to avoid fishey effect
		int r = this->checkCellValue(p);


		if (true)
		{
			float hp = dScreen * (hMur / d);

			vec2 start(i, resY / 2 - (hp / 2));
			vec2 end(i, resY / 2 + (hp / 2));

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