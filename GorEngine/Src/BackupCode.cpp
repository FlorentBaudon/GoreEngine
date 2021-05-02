// #include <GL/glew.h>
// #include <glm/glm.hpp>
//
// #include "RaycastTools.h"
//
// #define PI 3.1415f
//
// using namespace glm;
//
// void drawPoint(vec2 p, vec3 color = vec3(0,1,1))
// {
// 	glColor3f(color.r, color.g, color.b);
// 	glPointSize(8);
// 	glBegin(GL_POINTS);
// 	glVertex2f(p.x, p.y);
// 	glEnd();
// }
//
//
// //from 3D Sage
// void test(vec2 dir, float angle, int px, int py, int map[], int mapX, int mapY)
// {
// 	int r,mx,my,mp,dof = 0,  side=0;
// 	float vx,vy,rx,ry,xo,yo,disV = 0,disH;
// 	
// 	float ra = angle;
// 	
// 	float Tan=tan(ra);
// 	
// 	if(cos(ra)> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
// 	else if(cos(ra)<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
// 	else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  
// 	
// 	while(dof<8) 
// 	{ 
// 		mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
// 		if(mp>0 && mp<mapX*mapY && map[mp]==1)
// 		{
// 			dof=8; disV=cos(ra)*(rx-px)-sin(ra)*(ry-py);
// 			drawPoint(vec2(rx, ry), vec3(0,1,0));
// 		}//hit         
// 		else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
// 	}
//
// 	dof=0; disH=100000;
// 	Tan=1.0/Tan; 
// 	if(sin(ra)> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up 
// 	else if(sin(ra)<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
// 	else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right
//  
// 	while(dof<8) 
// 	{ 
// 		mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                          
// 		if(mp>0 && mp<mapX*mapY && map[mp]==1)
// 		{
// 			dof=8; disH=cos(ra)*(rx-px)-sin(ra)*(ry-py);
// 			drawPoint(vec2(rx, ry));
// 		}//hit         
// 		else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
// 	}
//
// }
//
// //Other version of my code
// void FindHori(vec2 pos, vec2 dir, float angle, int cellSize, int map[], int mapSizeX, int mapSizeY)
// {
// 	// test(dir, angle, pos.x, pos.y, map, mapSizeX, mapSizeY);
// 	// return;
// 	
// 	int dof = 8;
// 	vec2 p = vec2(0,0); 
// 	vec2 delta = vec2(0, 0);
// 	
// 	//check horizontal
// 	if (dir.y < 0) //looking up
// 	{
// 		p.y = (floorf(pos.y/cellSize) )*cellSize - 0.0001f;
// 		p.x = (p.y - pos.y) / tan(-angle) + pos.x;
// 		drawPoint(p);
//
// 		//next intersect
// 		delta.y = - cellSize;
// 		delta.x = delta.y / tan(-angle);
// 	}
// 	else if (dir.y > 0) //looking down
// 	{
// 		p.y = (floorf(pos.y / cellSize)) * cellSize + cellSize;
// 		p.x = (p.y - pos.y) / tan(-angle) + pos.x;
// 		drawPoint(p);
// 	
// 		//next intersect
// 		delta.y = cellSize;
// 		delta.x = cellSize / tan(-angle);
// 	}
//
// 	printf("X : %f - Y : %f ", p.x, p.y);
// 	
// 	for(int i=0; i<dof; i++)
// 	{	
// 		int mapPosX = (floorf(p.x / cellSize));
// 		int mapPosY = (floorf(p.y / cellSize));
// 		int mapTabPos = mapPosY * mapSizeX + mapPosX;
//
// 		printf("Map - X : %d - Y : %d - tabIndex : %d ", mapPosX, mapPosY, mapTabPos);
// 		
// 		if(mapTabPos > (mapSizeX * mapSizeY)  || mapTabPos < 0)
// 		{
// 			return;
// 		}
// 		if(map[mapTabPos] > 0)
// 		{
// 			printf("map pos %d = %d \n", mapTabPos, map[mapTabPos]);
// 			drawPoint(p, vec3(1,0,0));
// 			return;
// 		}
// 		
// 		p = p + delta;
// 		drawPoint(p);
// 	}
// }