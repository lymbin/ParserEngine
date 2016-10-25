/*
 * hero.h
 *
 *  Created on: 12.05.2014
 *      Author: dmitry
 */

#ifndef HERO_H_
#define HERO_H_

class hero;

#include "PE.h"
#include "Character.h"
#include "game.h"

class game;
class cTexture;

const unsigned int MAX_HERO_ANIM_SPEED = 50;

class hero : public iCharacter
{
	game *mGame; // holds game instance pointer
public:
	hero(std::string asName = "Timmi", int hp = 100);
	//hero(cTexture *aStaticTexture, std::string asName = "Timmi", int hp = 100);
	//hero(std::string aTexturePath, std::string asName = "Timmi", int hp = 100);
	~hero();

	void Move(int aiDirection, int aiAnimation = ANIM_UNKNOWN, int aiAnimpos = 0);
	void Jump();
	void Sit();
	void Shoot();

	void OnDraw();		// Перерисовка
	void Reset();		// Сброс настроек
	void OnStart();		// Установка начальных данных
	void OnExit(); 		// Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update(); 		// Обновление данных
	void PostUpdate();	// Обновление данных объекта в конце выполнения общего update

	static void CollisionHandler(iCollisionBody *thatBody, PE_Rect CollidedRect, void *CollidedObject, void *data);
};


#endif /* HERO_H_ */
