/*
 * Collision.h
 *
 *  Created on: 04.06.2014
 *      Author: dmitry
 */

#ifndef COLLISION_H_
#define COLLISION_H_

class collision_AABB;
class collision_OBB;

// Наше физическое тело для столкновения
class cCollisionBody
{
	// Система простая и не учитывает ускорения, массу, трение,
	// упругость, инерцию и очень много других параметров,
	// а содержит лишь позицию тела

public:
	cCollisionBody();
	~cCollisionBody();

	PE_Rect mBox;
};

class cCollision
{
#ifdef DEBUGGING
	// Включена ли система
	bool IsEnabled;
#endif

public:
	cCollision();
	~cCollision();

	// Инициализация системы
	int Init();

	// Основная функция проверки столкновений двух тел
	static bool CheckCollision(cCollisionBody A, cCollisionBody B);
	// Функция проверки столкновений двух прямоугольников
	static bool CheckCollision(PE_Rect A, PE_Rect B);
};

// Классы, которые пока не используются
class collision_AABB
{
	// AABB бокс столкновений
public:
	collision_AABB();
	~collision_AABB();

	// Проверка столкновения
	bool OverlapsAABB(collision_AABB aabb);
	bool OverlapsOBB(collision_OBB obb);

	void SetAABBBox(PE_Rect NewBox);

	// Положение тела
	PE_Rect AABBBodyBox;
	PE_Point MinPoint, MaxPoint;
};
class collision_OBB
{
	// OBB бокс столкновений
public:
	collision_OBB();
	~collision_OBB();

	// Проверка столкновения
	//bool OverlapsAABB(collision_AABB aabb);
	//bool OverlapsOBB(collision_OBB obb);

	// Положение тела
	PE_Rect OBBBodyBox;

};
#endif /* COLLISION_H_ */
