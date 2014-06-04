/*
 * Collision.h
 *
 *  Created on: 04.06.2014
 *      Author: dmitry
 */

#ifndef COLLISION_H_
#define COLLISION_H_

#include <algorithm>

class iCollisionBody;
class iCollisionLayer;

typedef std::vector<iCollisionBody *> tpCollisionLayer;
typedef std::vector<iCollisionBody *>::iterator tpCollisionLayerIt;

typedef std::vector<iCollisionLayer *> tpCollisionLayers;
typedef std::vector<iCollisionLayer *>::iterator tpCollisionLayersIt;

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

	// Основная функция проверки столкновений двух тел
	static bool CheckCollision(cCollisionBody A, cCollisionBody B);
	// Функция проверки столкновений двух прямоугольников
	static bool CheckCollision(PE_Rect A, PE_Rect B);


	void AddCollisionBody(iCollisionBody *aBody);
	void AddCollisionLayer(iCollisionLayer *aLayer);

	void EraseCollisionBody(iCollisionBody *aBody);
	void EraseCollisionLayer(iCollisionLayer *aLayer);

	void DeleteAll();

	void UpdateBodyLayers(iCollisionBody *aBody);

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


class iCollisionBody : public cCollisionBody
{
private:
	cCollision *mpCollision;

protected:
	tpCollisionLayers mCollisionLayers;

public:
	iCollisionBody();
	virtual ~iCollisionBody();

	void Collide();

	void SetBox(float W, float H, float X, float Y);
	void SetBox(PE_Rect aBox);

	void SetCollisionsPointer(cCollision *apCollision);

	// Устанавливем слой
	void AddNewLayer(iCollisionLayer *apLayer);

	void UpdateLayers();

	void EraseLayer(iCollisionLayer *apLayer);

	PE_Rect 		GetBox();
	cCollisionBody 	*GetCollisionBody();
	cCollision	 	*GetCollisionPointer();

	virtual bool 	HandleCollisions(){return false;}
	virtual void 	CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data) {}

};






/*
// Старые классы, которые пока не используются, но могут понадобиться в дальнейшем
class collision_AABB;
class collision_OBB;
class collision_layer;
class collision_body;
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

protected:
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
	collision_body(int ColPass = COLLISION_UNPASSABLE, int ColType = eBodyPosition_Outside, int BVType = COLLISION_AABB);
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
	friend class collision_body;

protected:
	// Указатель на класс столкновений
	collision *Collision;

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

	// Тела в текущем слое
	std::vector< collision_body *> bodies;
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
	collision_body *NewCollisionBody(unsigned int LayerId, int ColPass = COLLISION_UNPASSABLE, int ColType = eBodyPosition_Outside, int BVType = COLLISION_AABB);

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
*/
#endif /* COLLISION_H_ */
