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

	mpGame->Graphics->DrawFilledRectangle(GetBox(), cColor(1.0f, 0.0f, 1.0f, 1.0f));

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
	PE_Rect Box = GetBox();
	// TODO: обновлять камеру тут

	SetBox(Box);
}


void cStaticBox::CollisionHandler(iCollisionBody *thisBody, PE_Rect CollidedRect, void *CollidedObject, void *data)
{
	if(!CollidedObject)
		return;

	iGameObject *aGameObject = (iGameObject *)CollidedObject;
	if(aGameObject->mGameObjectType == eGameObjectType_Character)
	{
		iCharacter *aCharacter = (iCharacter *)CollidedObject;
		if(aCharacter->mCharacterType == eCharacterType_Hero)
		{
			// Какая-то проверка на расположение шипов
			aGameObject->SetHitPoints((aGameObject->GetHealth()-1));
		}
		else if(aCharacter->mCharacterType == eCharacterType_Ally)
		{
			// Какая-то проверка на расположение шипов
			aGameObject->SetHitPoints((aGameObject->GetHealth()-1));
		}
	}
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
void cDynamicBox::Move(int alDirection, int alAnimation, int alAnimpos)
{

}
void cDynamicBox::CollisionHandler(iCollisionBody *, iCollisionBody *, PE_Rect, void *data)
{

}
