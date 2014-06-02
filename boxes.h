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

class iStaticObject;
class iDynamicObject;

class cStaticBox : public iStaticObject
{
public:
	cStaticBox(std::string asName = "", int alHitPoints = 100);
	~cStaticBox();

	void OnDraw();	// Перерисовка
	void Reset();		// Сброс настроек
	void OnStart();	// Установка начальных данных
	void OnExit();	// Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update();	// Обновление данных

	void CollisionHandler();
};

class cDynamicBox : public iDynamicObject
{
	cDynamicBox(std::string asName = "", int alHitPoints = 100);
	~cDynamicBox();

	void OnDraw();	// Перерисовка
	void Reset();		// Сброс настроек
	void OnStart();	// Установка начальных данных
	void OnExit();	// Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update();	// Обновление данных

	void CollisionHandler();
	void Move(int alDirection, int alAnimation, int alAnimpos);
	void Jump() {}
	void Sit(){}
	void Shoot(){}
};

#endif /* BOXES_H_ */
