/*
 * button.cpp
 *
 *  Created on: 28.05.2014
 *      Author: dmitry
 */

#include "widget.h"

cButton::cButton(float W, float H, float X, float Y):cWidget(eWidgetType_Button, W, H, X, Y)
{
	mbPressed = false;

	mpTexture = 0;
}
cButton::~cButton()
{

}

void cButton::SetImage(cTexture *aTexture)
{
	mpTexture = aTexture;
}

void cButton::OnInit()
{

}
void cButton::OnDraw()
{

}
void cButton::OnUpdate()
{

}

