#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GlewDrawFunctions.h"

void drawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color)
{
	glColor3f(color.r, color.g, color.b);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();
}

void drawPoint(glm::vec2 p, glm::vec3 color)
{
	glColor3f(color.r, color.g, color.b);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}