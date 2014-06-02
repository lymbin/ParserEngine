/*
 * GameObject.cpp
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#include "GameObject.h"

iGameObject::iGameObject(eGameObjectType aType, std::string asName, int alHP)
{
	mpGame = 0;
	mlVelocity = 0;
	mGameObjectType = aType;
	msName = asName;
	mlHitPoints = alHP;
}
iGameObject::~iGameObject()
{

}
void iGameObject::SetGame(game *apGame)
{
	mpGame = apGame;
}
void iGameObject::SetName(std::string asName)
{
	msName = asName;
}
void iGameObject::SetHitPoints(int alHP)
{
	mlHitPoints = alHP;
}
void iGameObject::SetVelocity(unsigned int alVelocity)
{
	mlVelocity = alVelocity;
}
unsigned int iGameObject::GetVelocity()
{
	return mlVelocity;
}
std::string iGameObject::GetName()
{
	return msName;
}
int	iGameObject::GetHealth()
{
	return mlHitPoints;
}
void iGameObject::Update()
{
	OnUpdate();
}
void iGameObject::Render()
{
	OnRender();
}

iCollisionBody::iCollisionBody()
{
	Box.X = 0;
	Box.Y = 0;
	Box.Heigth = 0;
	Box.Width = 0;

	body = new collision_body();
	body->SetAABBBox(Box);
}
iCollisionBody::~iCollisionBody()
{

}

void iCollisionBody::SetBox(float W, float H, float X, float Y)
{
	Box.Heigth = H;
	Box.Width = W;
	Box.X = X;
	Box.Y = Y;

	body->SetAABBBox(Box);
}

void iCollisionBody::SetBody(collision_body *apBody)
{
	/*
	if(body)
	{
		delete body;
	}
	body = apBody;
	*/
}

PE_Rect iCollisionBody::GetBox()
{
	return Box;
}
collision_body *iCollisionBody::GetCollisionBody()
{
	return body;
}
void iCollisionBody::Collide()
{
	CollisionHandler();
}

