#include "Square.h"

void Square::Draw()
{
	if (id == 'w')//wall
	{
		glColor3f(0.5f, 0.0f, 0.0f);
	}
	if (id == 'f')//floor
	{
		glColor3f(0.0f, 0.5f, 0.0f);
	}
	if (id == 'p')//player
	{
		glColor3f(0.25f, 1.0f, 1.0f);
	}
	if (id == 'g')//goal
	{
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	if (id == 'e')//end of player move
	{
		glColor3f(1.0f, 0.0f, 1.0f);
	}
	glBegin(GL_QUADS);
	glVertex2d(pos.x + 0.0f, pos.y + 0.0f);
	glVertex2d(pos.x + size, pos.y + 0.0f);
	glVertex2d(pos.x + size, pos.y + size);
	glVertex2d(pos.x + 0.0f, pos.y + size);
	glEnd();
}

