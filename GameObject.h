/*
 * GameObject.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "PE.h"
#include "GameTypes.h"
#include <algorithm>

enum eGameObjectType
{
	eGameObjectType_Wall,			 // Стенка
	eGameObjectType_StaticObject,  	// Обычные блоки, которые не меняют своего положения на экране
	eGameObjectType_DynamicObject, 	// Двигающиеся блоки
	eGameObjectType_Character,	 	// Персонажи
	eGameObjectType_Ship,			 // Корабли
	eGameObjectType_LastEnum
};


// Тестовый класс
class iAnimed
{
protected:
	std::map <int, sAnim > mAnims;
	std::map <int, sAnim >::iterator mAnimIter;

public:
	iAnimed(){mAnims.clear();}
	virtual ~iAnimed(){}

	virtual void Move(int alDirection, int alAnimation, int alAnimpos) {}
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}
};

class iGameObject : public iCollisionBody, public iUpdateable
{
protected:
	int mlHitPoints;
	unsigned int mlVelocity;

public:
	iGameObject(eGameObjectType aType, std::string asName, int alHitPoints);
	virtual ~iGameObject();

	virtual void OnDraw() {}	// Перерисовка
	virtual void Reset() {}		// Сброс настроек
	virtual void OnStart() {}	// Установка начальных данных
	virtual void OnExit() {}	// Выполняем при выходе, на последней итерации фреймов или при удалении
	virtual void Update() {}	// Обновление данных

	void SetHitPoints(int alHP);
	void SetVelocity(unsigned int alVelocity);

	unsigned int GetVelocity();
	int	GetHealth();

	eGameObjectType mGameObjectType;

	//virtual void CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data) {}
};

class iStaticObject : public iGameObject
{
	sStaticTexture mTexture;

public:
	iStaticObject(std::string asName, int alHitPoints);
	virtual ~iStaticObject(){}

	virtual void OnDraw() {}	// Перерисовка
	virtual void Reset() {}		// Сброс настроек
	virtual void OnStart() {}	// Установка начальных данных
	virtual void OnExit() {}	// Выполняем при выходе, на последней итерации фреймов или при удалении
	virtual void Update() {}	// Обновление данных

	//virtual void CollisionHandler(){}
};

class iDynamicObject : public iGameObject, public iAnimed
{
	sStaticTexture mTexture;

public:
	iDynamicObject(std::string asName, int alHitPoints):iGameObject(eGameObjectType_DynamicObject, asName, alHitPoints)
	{
		mTexture.mfRotateDegrees = 0.0;
		mTexture.mfScaledMultiplier = 1.0;

		mTexture.mpTexture = 0;
	}
	virtual ~iDynamicObject(){}

	virtual void OnDraw() {}	// Перерисовка
	virtual void Reset() {}		// Сброс настроек
	virtual void OnStart() {}	// Установка начальных данных
	virtual void OnExit() {}	// Выполняем при выходе, на последней итерации фреймов или при удалении
	virtual void Update() {}	// Обновление данных

	virtual void CollisionHandler(){}

	virtual void Move(int alDirection, int alAnimation, int alAnimpos){}
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}
};

#endif /* GAMEOBJECT_H_ */
