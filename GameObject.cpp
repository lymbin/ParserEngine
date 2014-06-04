/*
 * GameObject.cpp
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#include "GameObject.h"

iGameObject::iGameObject(eGameObjectType aType, std::string asName, int alHP) : iUpdateable(asName)
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

iCollisionBody::iCollisionBody()
{
	mpBody = new cCollisionBody();

	mpBody->mBox.Heigth = 0;
	mpBody->mBox.Width = 0;
	mpBody->mBox.X = 0;
	mpBody->mBox.Y = 0;

	mpCollisions = 0;
}
iCollisionBody::~iCollisionBody()
{

}

void iCollisionBody::SetBox(float W, float H, float X, float Y)
{
	mpBody->mBox.Heigth = H;
	mpBody->mBox.Width = W;
	mpBody->mBox.X = X;
	mpBody->mBox.Y = Y;
}
void iCollisionBody::SetBox(PE_Rect aBox)
{
	mpBody->mBox.Heigth = aBox.Heigth;
	mpBody->mBox.Width = aBox.Width;
	mpBody->mBox.X = aBox.X;
	mpBody->mBox.Y = aBox.Y;
}
void iCollisionBody::SetCollisionsPointer(iCollisions *apCollisions)
{
	mpCollisions = apCollisions;
}
PE_Rect iCollisionBody::GetBox()
{
	return mpBody->mBox;
}
iCollisions *iCollisionBody::GetCollisionsPointer()
{
	return mpCollisions;
}
// Обрабатываем все столкновения
bool iCollisionBody::HandleCollisions()
{
	bool Result = false;
	UpdateLayers();

	tpCollisionLayersIt LayersIt = mCollisionLayers.begin();
	for( ; LayersIt != mCollisionLayers.end(); ++LayersIt)
	{
		tpCollisionLayerIt BodiesIt = (*LayersIt)->mCollisionLayer.begin();
		for( ; BodiesIt != (*LayersIt)->mCollisionLayer.end(); ++BodiesIt)
		{
			if(this->mpBody == (*BodiesIt)->mpBody)
				continue;
			if(cCollision::CheckCollision(*this->mpBody, *(*BodiesIt)->mpBody))
			{
				Result = true;
			}
		}
	}
	return Result;
}
void iCollisionBody::Collide()
{
	//CollisionHandler(iGameObject *Collider, iGameObject *CollSurface, void * data);
}

// Устанавливем слой
void iCollisionBody::AddNewLayer(iCollisionLayer *apLayer)
{
	mCollisionLayers.push_back(apLayer);
}

void iCollisionBody::UpdateLayers()
{
	tpCollisionLayersIt It = mCollisionLayers.begin();
	for( ; It != mCollisionLayers.end(); ++It)
	{
		if((*It)->CheckBodyInLayer(this) == eBodyPosition_Outside)
		{
			(*It)->EraseBody(this);
			EraseLayer((*It));
		}
	}
}

void iCollisionBody::EraseLayer(iCollisionLayer *apLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mCollisionLayers.begin(), mCollisionLayers.end(), apLayer);

	if(It != mCollisionLayers.end())
	{
		mCollisionLayers.erase(It);
	}
}

iStaticObject::iStaticObject(std::string asName, int alHitPoints):iGameObject(eGameObjectType_StaticObject, asName, alHitPoints)
{
	mTexture.mfRotateDegrees = 0.0;
	mTexture.mfScaledMultiplier = 1.0;

	mTexture.mpTexture = 0;
}

iCollisionLayer::iCollisionLayer(PE_Rect aBorder)
{
	Border.Heigth = aBorder.Heigth;
	Border.Width = aBorder.Width;
	Border.X = aBorder.X;
	Border.Y = aBorder.Y;
}
iCollisionLayer::iCollisionLayer(GLfloat W, GLfloat H, GLfloat X, GLfloat Y)
{
	Border.Heigth = H;
	Border.Width = W;
	Border.X = X;
	Border.Y = Y;
}
iCollisionLayer::~iCollisionLayer()
{
	mCollisionLayer.clear();
}

// Добавляем тело в слой
void iCollisionLayer::AddCollisionBody(iCollisionBody *body)
{
	mCollisionLayer.push_back(body);
}

// Проверяем где именно находится заданное тело, относительно слоя
int iCollisionLayer::CheckBodyInLayer(iCollisionBody *body)
{
	float leftBox, leftChecked;
	float rightBox, rightChecked;
	float	topBox, topChecked;
	float bottomBox, bottomChecked;

	// Параметры границы слоя
	leftBox = Border.X;
	rightBox = Border.X + Border.Width;
	topBox = Border.Y;
	bottomBox = Border.Y + Border.Heigth;

		// Простое сравнение AABB
		leftChecked = body->GetBox().X;
		rightChecked = body->GetBox().X + body->GetBox().Width;
		topChecked = body->GetBox().Y;
		bottomChecked = body->GetBox().Y + body->GetBox().Heigth;

		if((bottomBox >= bottomChecked) && (topBox <= topChecked) && (rightBox >= rightChecked) && (leftBox <= leftChecked))
		{
			// Тело внутри слоя
			return eBodyPosition_Inside;
		}
		else
		{
			// Ищем столкновения - можно сделать какую-нибудь общую функцию для поиска столкновений 2-х прямоугольников
			if(!collision::CheckCollision(Border, body->GetBox()))
			{
				return eBodyPosition_Outside;
			}
			else
			{
				// Тело соприкасается со слоем и входит одновременно в несколько слоёв и надо искать столкновения во всех
				return eBodyPosition_Intersect;
			}
		}

	return eBodyPosition_Outside;
}

// Удаляем тело из слоя
void iCollisionLayer::EraseBody(iCollisionBody *body)
{
	tpCollisionLayerIt It;
	It = std::find(mCollisionLayer.begin(), mCollisionLayer.end(), body);

	if(It != mCollisionLayer.end())
	{
		mCollisionLayer.erase(It);
	}
}

// Задаём границы слоя
void iCollisionLayer::SetLayerBorder(PE_Rect aBorder)
{
	Border.Heigth = aBorder.Heigth;
	Border.Width = aBorder.Width;
	Border.X = aBorder.X;
	Border.Y = aBorder.Y;
}

// Получаем границы слоя
PE_Rect iCollisionLayer::GetLayerBorder()
{
	return Border;
}

iCollisions::iCollisions()
{
	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}
iCollisions::~iCollisions()
{
	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}

void iCollisions::AddCollisionBody(iCollisionBody *aBody)
{
	tpCollisionLayerIt It;
	It = std::find(mAllCollisionBodies.begin(), mAllCollisionBodies.end(), aBody);

	if(It == mAllCollisionBodies.end())
	{
		mAllCollisionBodies.push_back(aBody);
	}
}
void iCollisions::AddCollisionLayer(iCollisionLayer *aLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mAllCollisionLayers.begin(), mAllCollisionLayers.end(), aLayer);

	if(It == mAllCollisionLayers.end())
	{
		mAllCollisionLayers.push_back(aLayer);
	}
}

void iCollisions::EraseCollisionBody(iCollisionBody *aBody)
{
	tpCollisionLayerIt It;
	It = std::find(mAllCollisionBodies.begin(), mAllCollisionBodies.end(), aBody);

	if(It != mAllCollisionBodies.end())
	{
		mAllCollisionBodies.erase(It);
	}
}
void iCollisions::EraseCollisionLayer(iCollisionLayer *aLayer)
{
	tpCollisionLayersIt It;
	It = std::find(mAllCollisionLayers.begin(), mAllCollisionLayers.end(), aLayer);

	if(It != mAllCollisionLayers.end())
	{
		mAllCollisionLayers.erase(It);
	}
}

void iCollisions::DeleteAll()
{
	tpCollisionLayersIt LayerIt = mAllCollisionLayers.begin();
	tpCollisionLayerIt BodyIt = mAllCollisionBodies.begin();

	for( ; LayerIt != mAllCollisionLayers.end(); ++LayerIt)
	{
		delete (*LayerIt);
	}

	for( ; BodyIt != mAllCollisionBodies.end(); ++BodyIt)
	{
		delete (*BodyIt);
	}

	mAllCollisionLayers.clear();
	mAllCollisionBodies.clear();
}
