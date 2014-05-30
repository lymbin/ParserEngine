/*
 * LowLevelGraphics.cpp
 *
 *  Created on: 29.05.2014
 *      Author: dmitry
 */

#include "Graphics.h"

iLowLevelGraphics::iLowLevelGraphics()
{

}
iLowLevelGraphics::~iLowLevelGraphics()
{

}
// Рисуем каркас прямоугольника цветными линиями
void iLowLevelGraphics::DrawRectangle(float x, float y, float width, float height, cColor aCol)
{
	glPushMatrix();
	//TODO: доделать работу с камерой
	glLoadIdentity();

	SetClearColor(aCol);

	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	glEnd();

	glLoadIdentity();

	glPopMatrix();
}
void iLowLevelGraphics::DrawRectangle(PE_Rect aRect, cColor aCol)
{
	DrawRectangle(aRect.X, aRect.Y, aRect.Width, aRect.Heigth, aCol);
}

// Рисуем заполненный цветом прямоугольник
void iLowLevelGraphics::DrawFilledRectangle(float x, float y, float width, float height, cColor aCol)
{
	glLoadIdentity();

	SetClearColor(aCol);

	glBegin(GL_QUADS);
		glVertex3f(x, y, 0);
		glVertex3f(x + width, y, 0);
		glVertex3f(x + width, y + height, 0);
        glVertex3f(x, y + height, 0);
    glEnd();

    glLoadIdentity();
}
void iLowLevelGraphics::DrawFilledRectangle(PE_Rect aRect, cColor aCol)
{
	DrawFilledRectangle(aRect.X, aRect.Y, aRect.Width, aRect.Heigth, aCol);
}

// Рисуем линию
void iLowLevelGraphics::DrawLine(float x1, float y1, float x2, float y2, cColor aCol)
{
	glLoadIdentity();

	SetClearColor(aCol);

	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();

	glLoadIdentity();
}
void iLowLevelGraphics::DrawLine(PE_Point A, PE_Point B, cColor aCol)
{
	DrawLine(A.x, A.y, B.x, B.y, aCol);
}

