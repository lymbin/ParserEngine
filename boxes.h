/*
 * boxes.h
 *
 *  Created on: 02.06.2014
 *      Author: dmitry
 */

#ifndef BOXES_H_
#define BOXES_H_

#include "PE.h"
#include "GameObject.h"
#include "game.h"

class game;
class iStaticObject;
class iDynamicObject;

class cStaticBox : public iStaticObject
{
	game *mpGame;
public:
	cStaticBox(std::string asName = "", int alHitPoints = 100);
	~cStaticBox();

	void OnDraw();		// Перерисовка
	void Reset();		// Сброс настроек
	void OnStart();		// Установка начальных данных
	void OnExit();		// Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update();		// Обновление данных
	void PostUpdate(); 	// Обновление данных объекта в конце выполнения общего update

	void SetGame(game *apGame){mpGame = apGame;}
	static void CollisionHandler(iCollisionBody *thisBody, PE_Rect CollidedRect, void *CollidedObject, void *data);
};

class cDynamicBox : public iDynamicObject
{
	game *mpGame;
public:
	cDynamicBox(std::string asName = "", int alHitPoints = 100);
	~cDynamicBox();

	void OnDraw();		// Перерисовка
	void Reset();		// Сброс настроек
	void OnStart();		// Установка начальных данных
	void OnExit();		// Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update();		// Обновление данных
	void PostUpdate();	// Обновление данных объекта в конце выполнения общего update

	void Move(int alDirection, int alAnimation, int alAnimpos);
	void Jump() {}
	void Sit(){}
	void Shoot(){}

	void SetGame(game *apGame){mpGame = apGame;}
	static void CollisionHandler(iCollisionBody *, iCollisionBody *, PE_Rect, void *data);
};

#endif /* BOXES_H_ */
