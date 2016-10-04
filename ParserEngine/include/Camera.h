/*
 * Camera.h
 *
 *  Created on: 09.09.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef PARSERENGINE_INCLUDE_CAMERA_H_
#define PARSERENGINE_INCLUDE_CAMERA_H_

// Support only 2D camera.
#include <GL/gl.h>
#include <GL/glu.h>

enum eCameraMoveType {
	eCameraMoveTypeImmediatly = 0,
	eCameraMoveTypeInner,
	eCameraMoveTypeFluid,
	eCameraMoveTypeLastEnum
};

/*
enum eCameraType {
	eCameraTypeBoxCentered,
	eCameraType
};
*/

class cCamera
{
	GLfloat gCameraX;
	GLfloat gCameraY;

	unsigned int mCameraMoveType;
	int mInnerSize;
	float mFluidSize;

public:
	cCamera();
	~cCamera();

	void MoveTo(GLfloat x, GLfloat y);
	void SetPosition(GLfloat x, GLfloat y);

	GLfloat GetXposition();
	GLfloat GetYposition();

	void SetCameraMoveType(eCameraMoveType aType);
	void SetInnerSize(int aSize);
	void SetFluidSize(float aSize);

	eCameraMoveType GetCameraMoveType();
	float GetFluidSize();
	int GetInnerSize();
};

#endif /* PARSERENGINE_INCLUDE_CAMERA_H_ */
