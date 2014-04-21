/*
 * game.h
 *
 *  Created on: 18.04.2014
 *      Author: dmitry
 */

#ifndef GAME_H_
#define GAME_H_

#include "engine.h"

//Нижние дефайны пока никак не используем
#define LEVEL_MAINMENU  0
#define LEVEL_GAMELEVEL 1


struct MainMenu
{
	image *background;
	//image *button_start;
	//image *button_exit;
};

class game: public engine
{
	static bool quit;
	MainMenu Mmenu;
public:
	int LoadTextures();
	void FreeTextures();
	void update();
	void render();
	void MainLoop();
	game();
	~game();
};



#endif /* GAME_H_ */
