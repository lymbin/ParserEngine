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
	iAnimed();
	virtual ~iAnimed();

	virtual void Move(int alDirection, int alAnimation, int alAnimpos) {}
	virtual void Jump() {}
	virtual void Sit(){}
	virtual void Shoot(){}
};

class iCollisionBody
{
protected:
	PE_Rect 		Box;
	collision_body *body;

public:
	iCollisionBody();
	virtual ~iCollisionBody();

	void Collide();

	void SetBox(float W, float H, float X, float Y);
	void SetBody(collision_body *apBody);

	PE_Rect 		GetBox();
	collision_body *GetCollisionBody();
	virtual void CollisionHandler() {}
};


class iGameObject : public iCollisionBody
{
protected:
	game *mpGame;
	std::string msName;

	eGameObjectType mGameObjectType;

	int mlHitPoints;
	unsigned int mlVelocity;

public:
	iGameObject(eGameObjectType aType, std::string asName, int alHitPoints);
	virtual ~iGameObject();

	void Update();
	void Render();

	void SetGame(game *apGame);
	void SetName(std::string asName);
	void SetHitPoints(int alHP);
	void SetVelocity(unsigned int alVelocity);

	unsigned int GetVelocity();
	std::string GetName();
	int	GetHealth();

	virtual void OnUpdate(){}
	virtual void OnRender(){}
};

class cStaticObject : public iGameObject
{
	sStaticTexture mTexture;

public:
	cStaticObject(std::string asName = "", int alHitPoints = 100);
	~cStaticObject();

	void OnUpdate();
	void OnRender();

	void CollisionHandler();
};

class cDynamicObject : public iGameObject, public iAnimed
{
	sStaticTexture mTexture;

	std::map <int, sAnim > mAnims;
	std::map <int, sAnim >::iterator mAnimIter;

public:
	cDynamicObject(std::string asName = "", int alHitPoints = 100);
	~cDynamicObject();

	void OnUpdate();
	void OnRender();

	void CollisionHandler();

	void Move(int alDirection, int alAnimation, int alAnimpos);
};
#endif /* GAMEOBJECT_H_ */
