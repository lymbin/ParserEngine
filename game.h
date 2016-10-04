/*
 * game.h
 *
 *  Created on: 18.04.2014
 *      Author: dmitry
 */

#ifndef GAME_H_
#define GAME_H_

#include "PE.h"
#include "GameTypes.h"
#include "Inventory.h"
#include "Character.h"
#include "boxes.h"
#include "hero.h"

class hero;
class cStaticBox;

//Нижние дефайны пока никак не используем
#define LEVEL_MAINMENU  0
#define LEVEL_GAMELEVEL 1

const int GAME_GUI_HEIGTH = 200;

const int GAME_HERO_HP = 100;
const std::string GAME_HERO_NAME = "Timmi";


struct MainMenu
{
	cTexture	*background;
	font 		*MainMenuFont;
	//image 	*button_start;
	//image 	*button_exit;
};

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
	static bool quit;
	static bool timer_running;
	MainMenu Mmenu;
	game_gui *Gui;
	hero *Hero; // TODO: rename to player
	iCollisionLayer *layer;
	cCollision *Collision;
	cStaticBox *StaticBox;
	font 	*DynamicTextFont;
	cMusic *MainMusic;


public:
	game();
	~game();

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


};
#endif /* GAME_H_ */
