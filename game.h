/*
 * game.h
 *
 *  Created on: 18.04.2014
 *  	Author: Dmitry Kilchanov <dmitrykilchanov@gmail.com>
 *
 *	Copyright 2014-2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com> - Mind Walkers
 *
 *	This file is part of Parser Engine
 */

#ifndef GAME_H_
#define GAME_H_

#include "PE.h"
#include "GameTypes.h"
#include "Inventory.h"
#include "Character.h"
#include "boxes.h"
#include "hero.h"
#include "GameLevels.h"

class hero;
class cStaticBox;
class cGameLevel;
class cMainMenu;

//Нижние дефайны пока никак не используем
#define LEVEL_MAINMENU  0
#define LEVEL_GAMELEVEL 1

const int GAME_GUI_HEIGTH = 200;

const int GAME_HERO_HP = 100;
const std::string GAME_HERO_NAME = "Timmi";

struct bottom_gui
{
	cTexture 	*texture;
	font 		*StaticTextFont;
	font 		*DynamicTextFont;
};
class game_gui
{
	bottom_gui bottom;
public:
	game_gui();
	~game_gui();
	void update();
	void render();
};
class game: public engine
{
	static bool 		quit;
	static bool 		timer_running;

	game_gui 			*Gui;
	font 				*DynamicTextFont;

	// Level list
	cGameLevel			*mGameLevel;
	cMainMenu			*mMainMenuLevel;

	game();
	~game();

public:
	static game *Instance()
	{
		if (!self)
			self = new game();

		return self;
	}

	static bool DeleteInstance()
	{
		if (self)
		{
			delete self;
			self = 0;
			return true;
		}
		return false;
	}

	cCollision 	  	*Collision;
	font 			*textFont;
	bool			mIsLevelChanged;

	// Определяем объекты
	int CreatingObjects();

	// Загружаем текстуры
	int LoadTextures();

	// Освобождаем текстуры
	void FreeTextures();

	// Освобождаем объекты
	void FreeObjects();

	// Обновляем координаты текстур и объектов
	void update();

	// Обновление данных объекта в конце выполнения общего update
	void PostUpdate();

	// Отрисовываем текстуры и объекты
	void render();

	// Основной цикл
	void MainLoop();

	void InitLevels();

	void MainQuit();

protected:
	static game *self;

};
#endif /* GAME_H_ */
