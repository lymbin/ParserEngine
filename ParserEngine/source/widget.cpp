/*
 * widget.cpp
 *
 *  Created on: 28.05.2014
 *      Author: dmitry
 */

#include "widget.h"

cWidget::cWidget(eWidgetType aType, float W, float H, float X, float Y)
{
	mbHasParent = false;
	mbMouseIsOver = true;
	mbVisible = true;

	mpGui = 0;
	mpParent = 0;

	mType = aType;
	msName = "";

	WidgetBox.Width = W;
	WidgetBox.Heigth = H;
	WidgetBox.X = X;
	WidgetBox.Y = Y;

	mCallbackList.clear();
}
cWidget::~cWidget()
{
	mCallbackListIt = mCallbackList.begin();
	for( ; mCallbackListIt != mCallbackList.end(); ++mCallbackListIt)
	{
		if(mCallbackListIt->second)
			delete mCallbackListIt->second;
	}
	mCallbackList.clear();
}
void cWidget::AddCallback(eGuiMessage aMessage, void *cbObject, tCallbackFunc cbFunc)
{
	cWidgetCallback *NewCallback = new cWidgetCallback(cbObject, cbFunc);
	if(mCallbackList[aMessage])
	{
		delete mCallbackList[aMessage];
	}
	mCallbackList[aMessage] = NewCallback;
}

cWidget *cWidget::GetParent()
{
	return mpParent;
}

bool cWidget::IsVisible()
{
	return mbVisible;
}
bool cWidget::HasParent()
{
	return mbHasParent;
}


