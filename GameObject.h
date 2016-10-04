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
	eGameObjectType_Wall,			// Стены, неразрушаемые объекты.
	eGameObjectType_StaticObject,  	// Статичные объекты, не меняющие своего положения на экране со временем.
	eGameObjectType_DynamicObject, 	// Динамичные объекты, двигающиеся по экрану.
	eGameObjectType_Character,	 	// Персонажи
	eGameObjectType_Ship,			// TODO: not used - remove.
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

	//virtual void Animate(int alAnimation, int alAnimpos) {} // TODO: add this function to animate action for game object.
	virtual void Move(int alDirection, int alAnimation, int alAnimpos) {} // One of the standard action in animation.

	// TODO: remove this from iAnimed class and put it to iCharacter or some else.
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}
};

class iGameObject : public iCollisionBody, public iUpdatable
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
	virtual void PostUpdate() {}// Обновление данных объекта в конце выполнения общего update

	// TODO: remove velocity, cause static objects does not have velocity.
	void SetHitPoints(int alHP);
	void SetVelocity(unsigned int alVelocity);

	unsigned int GetVelocity();
	int	GetHealth();

	eGameObjectType mGameObjectType;

	//virtual void CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data) {}
};

// TODO: добавить анимации
// Статичные объекты, не меняющие своего положения на экране со временем. Могут иметь анимации.
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
	virtual void PostUpdate() {}// Обновление данных объекта в конце выполнения общего update

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
	virtual void PostUpdate() {}// Обновление данных объекта в конце выполнения общего update

	virtual void CollisionHandler(){}

	//virtual void Animate(int alAnimation, int alAnimpos) {} // TODO: add this function to animate action for game object.
	virtual void Move(int alDirection, int alAnimation, int alAnimpos){}
	// TODO: remove this from iAnimed class and put it to iCharacter or some else.
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}
};

#endif /* GAMEOBJECT_H_ */
