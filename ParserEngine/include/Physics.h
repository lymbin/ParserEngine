/*
 * Physics.h
 *
 *  Created on: 30.05.2014
 *      Author: dmitry
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "SystemConstants.h"
#include "GraphicTypes.h"
#include "PhysicsTypes.h"

class collision_AABB;
class collision_OBB;
class collision_body;
class collision_layer;
class collision;


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
class collision_body: public collision_OBB, public collision_AABB
{
	// Тело столкновений - по-сути коробка, содержащаяся в слоях(слое) столкновений
	friend class collision;
	friend class collision_layer;

	// Указатель на класс столкновений
	collision *Collision;

	// Вектор указателей на все слои в которые входит(пересекается) данное тело
	std::vector< collision_layer *> CurrentLayers;

	// Проходимо ли тело
	unsigned int CollisionPass;

	// Тип столкновения
	int CollisionType;

	// Тип рамки столкновения
	int BoundingVolumeType;

public:
	collision_body(int ColPass = COLLISION_UNPASSABLE, int ColType = COLLISION_OUTSIDE, int BVType = COLLISION_AABB);
	~collision_body();

	// Устанавливем слой
	void AddNewLayer(collision_layer *layer);

	// Удаляем слой
	void RemoveLayer(unsigned int LayerID = -1);

	// Удаляем все слои
	void ClearLayers();

	// Проверяем содержание в слоях
	void UpdateLayers();

	bool CheckCollision();

	// Устанавливаем тип столкновения
	void SetCollisionType(int Type);

	// Устанавливаем тип столкновения
	void SetCollisionPass(unsigned int Pass);

	// Устанавливаем тип коробки
	void SetBoundingVolumeType(int BVType);

	// Получаем тип тела
	unsigned int IsPassable();

	// Получаем тип столкновения
	int GetCollisionType();

	// Получаем тип коробки столкновений
	int GetBoundingVolumeType();

	// Получаем определённый слой столкновений
	collision_layer *GetCollisionLayer(unsigned int LayerID = 0);

	// Получаем размер вектора слоёв
	int GetLayersSize();
};
class collision_layer
{
	// Класс слоя столкновений хранит тела, которые могут сталкиваться в ограниченной области
	friend class collision;

	// Указатель на класс столкновений
	collision *Collision;

	// Тела в текущем слое
	std::vector< collision_body *> bodies;

	// Прямоугольная рамка слоя
	PE_Rect LayerBorder;

	// Оптимизируем систем столкновений - сортировка, удаление "лишних" тел столкновения
	void OptimizeCollisions();
	void SortCollisions();

public:
	collision_layer(PE_Rect Box);
	collision_layer(GLfloat W = SYS_WIDTH, GLfloat H = SYS_HEIGTH, GLfloat X = 0, GLfloat Y = 0);
	~collision_layer();

	// Добавляем тело в слой
	void AddCollisionBody(collision_body *body);

	// Проверяем где именно находится заданное тело, относительно слоя
	int CheckBodyInLayer(collision_body *body);

	// Проверяем столкновение тела с телами из слоя
	bool CheckCollision(collision_body *body);

	// Удаляем тело из слоя
	void EraseBody(collision_body *body);

	// Функции полного удаления тел
	void DeleteAll();
	void DeleteCollisionBody(collision_body *body);

	// Задаём границы слоя
	void SetLayerBorder(PE_Rect Border);

	// Получаем границы слоя
	PE_Rect GetLayerBorder();
};
class collision
{
	// Класс, хранящий все слои столкновений и управляющий всей системой столкновений

	// Все тела в системе столкновений
	std::vector< collision_layer *> layers;

#ifdef DEBUGGING
	// Включена ли система
	bool IsEnabled;
#endif

public:
	collision();
	~collision();

	// Инициализация системы
	int init();

	// Добавляем новое тело в систему столкновений
	collision_body *NewCollisionBody(unsigned int LayerId, int ColPass = COLLISION_UNPASSABLE, int ColType = COLLISION_OUTSIDE, int BVType = COLLISION_AABB);

	// Добавляем новый слой в систему столкновений
	collision_layer *NewCollisionLayer(GLfloat W = SYS_WIDTH, GLfloat H = SYS_HEIGTH, GLfloat X = 0, GLfloat Y = 0);

	// Добавляем тело в слой с указанным id
	void AddBodyToLayer(collision_body *body, unsigned int LayerId);

	// Удаляем тело из слоя с указанным id
	void EraseBodyFromLayer(collision_body *body, unsigned int LayerId);

	// Очищаем выбранный слой
	void EraseCollisionLayer(unsigned int LayerId);
	void EraseCollisionLayer(collision_layer *layer);

	// Общая функция проверки столкновений двух прямоугольников - является базовой для многих функций проверки столкновений в других классах системы
	static bool CheckCollision(PE_Rect A, PE_Rect B);

	// Ищем слои для тела и добавляем их
	void NewLayersInBody(collision_body *body);

	// Удаление столкновений из системы
	void DeleteAll();

#ifdef DEBUGGING
	// Включение/выключение системы столкновений
	void SwitchCollision();
#endif
};


#endif /* PHYSICS_H_ */
