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

const unsigned int MAX_HERO_ANIM_SPEED = 30;

class hero : public iCharacter
{
	game *mpGame;
public:
	hero(std::string nam = "Timmi", int hp = 100);
	~hero();

	void Move(int aiDirection, int aiAnimation = ANIM_UNKNOWN, int aiAnimpos = 0);
	void Jump();
	void Sit();
	void Shoot();

	void OnDraw();	// Перерисовка
	void Reset();	// Сброс настроек
	void OnStart();	// Установка начальных данных
	void OnExit(); // Выполняем при выходе, на последней итерации фреймов или при удалении
	void Update(); // Обновление данных

	void SetGame(game *apGame);
	void CollisionHandler(iCollisionBody *Collider, iCollisionBody *CollSurface, void *data);
};


#endif /* HERO_H_ */
