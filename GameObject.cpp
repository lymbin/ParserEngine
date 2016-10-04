/*
 * GameObject.cpp
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#include "GameObject.h"

iGameObject::iGameObject(eGameObjectType aType, std::string asName, int alHP) : iUpdatable(asName)
{
	mlVelocity = 0;
	mGameObjectType = aType;
	mlHitPoints = alHP;
}
iGameObject::~iGameObject()
{

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
int	iGameObject::GetHealth()
{
	return mlHitPoints;
}
iStaticObject::iStaticObject(std::string asName, int alHitPoints):iGameObject(eGameObjectType_StaticObject, asName, alHitPoints)
{
	mTexture.mfRotateDegrees = 0.0;
	mTexture.mfScaledMultiplier = 1.0;

	mTexture.mpTexture = 0;
}
