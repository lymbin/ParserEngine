/*
 * camera.cpp
 *
 *  Created on: 08.05.2014
 *      Author: dmitry
 */

#include "engine.h"

camera::camera()
{
	gCameraX = 0;
	gCameraY = 0;
}
camera::~camera()
{

}
void camera::MoveTo(GLfloat x, GLfloat y)
{
	// TODO: протестить
	gCameraX -= x;
	gCameraY -= y;
}
void camera::SetPosition(GLfloat x, GLfloat y)
{
	// TODO: протестить
	gCameraX = -x;
	gCameraY = -y;
}

GLfloat camera::GetXposition()
{
	return gCameraX;
}
GLfloat camera::GetYposition()
{
	return gCameraY;
}
