/*
 * camera.cpp
 *
 *  Created on: 08.05.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cCamera::cCamera()
{
	gCameraX = 0;
	gCameraY = 0;
	mCameraMoveType = eCameraMoveTypeImmediatly;
	mInnerSize = 0;
	mFluidSize = 1.0;
}

//-----------------------------------------------------------------------

cCamera::~cCamera()
{

}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cCamera::MoveTo(GLfloat x, GLfloat y)
{
	// TODO: протестить
	gCameraX -= x;
	gCameraY -= y;
}

//-----------------------------------------------------------------------

void cCamera::SetPosition(GLfloat x, GLfloat y)
{
	// TODO: протестить
	gCameraX = -x;
	gCameraY = -y;
}

//-----------------------------------------------------------------------

GLfloat cCamera::GetXposition()
{
	return gCameraX;
}

//-----------------------------------------------------------------------

GLfloat cCamera::GetYposition()
{
	return gCameraY;
}

//-----------------------------------------------------------------------

void cCamera::SetCameraMoveType(eCameraMoveType aType)
{
	mCameraMoveType = aType;
}

//-----------------------------------------------------------------------

void cCamera::SetInnerSize(int aSize)
{
	mInnerSize = aSize;
}

//-----------------------------------------------------------------------

void cCamera::SetFluidSize(float aSize)
{
	if (aSize != 0.0)
		mFluidSize = aSize;
}

//-----------------------------------------------------------------------

eCameraMoveType cCamera::GetCameraMoveType()
{
	return (eCameraMoveType)mCameraMoveType;
}

//-----------------------------------------------------------------------

float cCamera::GetFluidSize()
{
	return mFluidSize;
}

//-----------------------------------------------------------------------

int cCamera::GetInnerSize()
{
	return mInnerSize;
}
