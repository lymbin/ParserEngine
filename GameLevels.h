/*
 * GameLevels.h
 *
 *  Created on: 23.10.2016
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef GAMELEVELS_H_
#define GAMELEVELS_H_

#include "PE.h"
#include "GameTypes.h"
#include "Inventory.h"
#include "Character.h"
#include "boxes.h"
#include "hero.h"

class hero;
class cStaticBox;
class game_gui;

class cGameLevel: public cLevel
{
	game_gui 			*Gui;
	hero 				*Player; // TODO: rename to player
	iCollisionLayer 	*CollisionLayer;
	cStaticBox 			*StaticBox;
	font 				*DynamicTextFont;
	cPlaylist			*LevelPlaylist;
	cCollision 	  		*Collision;
	cTexture			*PlayerStaticTexture;

public:
	cGameLevel(std::string aLevelName);
	~cGameLevel();

	int Init();

	int CleanUp();

	// Определяем объекты
	int  CreatingObjects();

	// Загружаем текстуры
	int  LoadTextures();

	// Освобождаем текстуры
	void FreeTextures();

	// Освобождаем объекты
	void FreeObjects();

	// Обновляем координаты текстур и объектов
	void Update();

	// Обновление данных объекта в конце выполнения общего update
	void PostUpdate();

	// Отрисовываем текстуры и объекты
	void Render();

	void PostRender();
};

struct MainMenu
{
	cTexture	*background;
	font 		*MainMenuFont;
	//image 	*button_start;
	//image 	*button_exit;
};

class cMainMenu: public cLevel
{
	MainMenu 	MMenu;
public:
	cMainMenu();
	~cMainMenu();

	int Init();

	int CleanUp();

	// Определяем объекты
	int  CreatingObjects();

	// Загружаем текстуры
	int  LoadTextures();

	// Освобождаем текстуры
	void FreeTextures();

	// Освобождаем объекты
	void FreeObjects();

	// Обновляем координаты текстур и объектов
	void Update();

	// Обновление данных объекта в конце выполнения общего update
	void PostUpdate();

	// Отрисовываем текстуры и объекты
	void Render();

	void PostRender();
};

#endif /* GAMELEVELS_H_ */
