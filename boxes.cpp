/*
 * boxes.cpp
 *
 *  Created on: 02.06.2014
 *      Author: dmitry
 */

#include "boxes.h"

using namespace std;

cStaticBox::cStaticBox(std::string asName, int alHitPoints) : iStaticObject(asName, alHitPoints)
{
	mpGame = 0;
}
cStaticBox::~cStaticBox()
{

#ifdef DEBUG_SYS
	cout << "One StaticBox deleted..." << endl;
#endif
}

void cStaticBox::OnDraw()
{
	if(!mpGame && !mpGame->Graphics)
		return;

	mpGame->Graphics->DrawFilledRectangle(Box.X, Box.Y, Box.Width, Box.Heigth, cColor(1.0f, 0.0f, 1.0f, 1.0f));

}
void cStaticBox::Reset()
{

}
void cStaticBox::OnStart()
{

}
void cStaticBox::OnExit()
{

}
void cStaticBox::Update()
{
	// TODO: обновлять камеру тут

}

void cStaticBox::CollisionHandler()
{

}

cDynamicBox::cDynamicBox(std::string asName, int alHitPoints) : iDynamicObject(asName, alHitPoints)
{
	mpGame = 0;
}
cDynamicBox::~cDynamicBox()
{

#ifdef DEBUG_SYS
	cout << "One DynamicBox deleted..." << endl;
#endif
}

void cDynamicBox::OnDraw()
{

}
void cDynamicBox::Reset()
{

}
void cDynamicBox::OnStart()
{

}
void cDynamicBox::OnExit()
{

}
void cDynamicBox::Update()
{

}

void cDynamicBox::CollisionHandler()
{

}
void cDynamicBox::Move(int alDirection, int alAnimation, int alAnimpos)
{

}
