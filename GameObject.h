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

class iGameObject;
class iCollisionBody;
class iCollisionLayer;

typedef std::vector<iCollisionBody *> tpCollisionLayer;
typedef std::vector<iCollisionBody *>::iterator tpCollisionLayerIt;

typedef std::vector<iCollisionLayer *> tpCollisionLayers;
typedef std::vector<iCollisionLayer *>::iterator tpCollisionLayersIt;

class iCollisions
{
public:
	iCollisions();
	~iCollisions();

	void AddCollisionBody(iCollisionBody *aBody);
	void AddCollisionLayer(iCollisionLayer *aLayer);

	void EraseCollisionBody(iCollisionBody *aBody);
	void EraseCollisionLayer(iCollisionLayer *aLayer);

	void DeleteAll();

	tpCollisionLayers mAllCollisionLayers;
	tpCollisionLayer mAllCollisionBodies;
};

class iCollisionLayer
{
	PE_Rect Border;
public:
	iCollisionLayer(PE_Rect aBorder);
	iCollisionLayer(GLfloat W = SYS_WIDTH, GLfloat H = SYS_HEIGTH, GLfloat X = 0, GLfloat Y = 0);
	~iCollisionLayer();

	// Добавляем тело в слой
	void AddCollisionBody(iCollisionBody *body);

	// Проверяем где именно находится заданное тело, относительно слоя
	int CheckBodyInLayer(iCollisionBody *body);

	// Удаляем тело из слоя
	void EraseBody(iCollisionBody *body);

	// Задаём границы слоя
	void SetLayerBorder(PE_Rect aBorder);

	// Получаем границы слоя
	PE_Rect GetLayerBorder();

	tpCollisionLayer mCollisionLayer;
};


class iCollisionBody
{
private:
	iCollisions *mpCollisions;

protected:
	cCollisionBody *mpBody;
	tpCollisionLayers mCollisionLayers;

public:
	iCollisionBody();
	virtual ~iCollisionBody();

	void Collide();
	bool HandleCollisions();

	void SetBox(float W, float H, float X, float Y);
	void SetBox(PE_Rect aBox);

	void SetCollisionsPointer(iCollisions *apCollisions);

	// Устанавливем слой
	void AddNewLayer(iCollisionLayer *apLayer);

	void UpdateLayers();

	void EraseLayer(iCollisionLayer *apLayer);

	PE_Rect 		GetBox();
	cCollisionBody 	*GetCollisionBody();
	iCollisions 	*GetCollisionsPointer();
	virtual void 	CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data) {}


};








class iGameObject : public iCollisionBody, public iUpdateable
{
protected:

	eGameObjectType mGameObjectType;

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

	virtual void CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data) {}
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

	virtual void CollisionHandler(){}
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
