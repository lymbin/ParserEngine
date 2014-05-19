/*
 * game.h
 *
 *  Created on: 18.04.2014
 *      Author: dmitry
 */

#ifndef GAME_H_
#define GAME_H_

#include "engine.h"
#include "GameTypes.h"
#include "Inventory.h"
#include "hero.h"


//Нижние дефайны пока никак не используем
#define LEVEL_MAINMENU  0
#define LEVEL_GAMELEVEL 1

const int GAME_GUI_HEIGTH = 200;

const int GAME_HERO_HP = 100;
const std::string GAME_HERO_NAME = "Timmi";

struct MainMenu
{
	image 	*background;
	text 	*title;
	//image *button_start;
	//image *button_exit;
};

class hero;


struct bottom_gui
{
	image *texture;
	text *static_text;
	text *dynamic_text;
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
	hero *Hero;

public:
	game();
	~game();

	int LoadTextures();
	void FreeTextures();
	void update();
	void render();
	void MainLoop();


};



#endif /* GAME_H_ */
